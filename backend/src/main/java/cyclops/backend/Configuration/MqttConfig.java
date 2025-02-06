package cyclops.backend.Configuration;

import java.net.InetAddress;

import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.integration.channel.DirectChannel;
import org.springframework.integration.core.MessageProducer;
import org.springframework.integration.mqtt.core.DefaultMqttPahoClientFactory;
import org.springframework.integration.mqtt.core.MqttPahoClientFactory;
import org.springframework.integration.mqtt.inbound.MqttPahoMessageDrivenChannelAdapter;
import org.springframework.integration.mqtt.outbound.MqttPahoMessageHandler;
import org.springframework.messaging.MessageChannel;
import org.springframework.messaging.handler.annotation.Header;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

import cyclops.backend.models.BatteryLevel;
import cyclops.backend.models.MappingValue;
import cyclops.backend.models.Message;
import cyclops.backend.services.BatteryLevelService;
import cyclops.backend.services.MappingValueService;
import cyclops.backend.services.MessageService;
import cyclops.backend.services.ReferencePointService;
import jakarta.annotation.PreDestroy;

import java.net.UnknownHostException;


@Configuration
public class MqttConfig {
    // "tcp://192.168.4.2:1883"
    private static final String BACKEND_IP = "192.168.4.2";
    private static final String[] serverUri = { "tcp://" + BACKEND_IP + ":1883" };
    private static final String[] RTOPICS = { "Mapping", "Messages", "Battery", "Barrier" };
    private static final String[] STOPICS = { "Instruction" };
    private static final String BACKEND_ID = "backend-service"; // ID único para el backend
    private static final int RETRY_INTERVAL_MS = 2000; // Tiempo entre intentos en milisegundos
    private volatile boolean running = true;

    private final MappingValueService mappingValueService;
    private final MessageService messageService;
    private final BatteryLevelService batteryLevelService;
    private final ReferencePointService referencePointService;

    public MqttConfig(MappingValueService mappingValueService, MessageService messageService,
            BatteryLevelService batteryLevelService, ReferencePointService referencePointService) {
        this.mappingValueService = mappingValueService;
        this.messageService = messageService;
        this.batteryLevelService = batteryLevelService;
        this.referencePointService = referencePointService;

    }

    @Bean
    public MqttPahoClientFactory mqttClientFactory() {
        waitForBroker();
        DefaultMqttPahoClientFactory factory = new DefaultMqttPahoClientFactory();
        MqttConnectOptions options = new MqttConnectOptions();
        options.setServerURIs(serverUri); // Broker MQTT
        options.setCleanSession(true);
        options.setAutomaticReconnect(false);
        options.setConnectionTimeout(10);
        factory.setConnectionOptions(options);
        return factory;
    }

    private void waitForBroker() {
        System.out.println("Esperando a que la IP del host sea " + BACKEND_IP + "...");
        while (!NetworkUtils.isMyIp(BACKEND_IP)) {
            try {
                InetAddress ip = InetAddress.getByName("host.docker.internal");
            System.out.println("IP real de la PC detectada desde Docker: " + ip.getHostAddress());
                System.out.println(
                        "La IP actual no es " + BACKEND_IP + ", reintentando en " + RETRY_INTERVAL_MS + "ms...");
                Thread.sleep(RETRY_INTERVAL_MS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                throw new IllegalStateException("Interrupción durante la espera de la IP correcta", e);
            }
            catch (UnknownHostException e) {
                System.err.println("Error: No se pudo obtener la IP de la PC. Asegúrate de que host.docker.internal está disponible.");
            }
            }
        System.out.println("La IP del host ahora es " + BACKEND_IP + ". Continuando...");
    }

    // SEND
    @Bean
    public MessageChannel mqttOutboundChannel() {
        return new DirectChannel();
    }

    // RECEIVE MQTT
    @Bean
    @ServiceActivator(inputChannel = "mqttOutboundChannel")
    public MqttPahoMessageHandler mqttMessageHandler() {
        MqttPahoMessageHandler messageHandler = new MqttPahoMessageHandler(BACKEND_ID + "-outbound",
                mqttClientFactory());
        messageHandler.setDefaultTopic(STOPICS[0]);

        return messageHandler;
    }

    // Escucha de mensajes entrantes en los tópicos
    @Bean
    public MessageProducer inbound() {
        MqttPahoMessageDrivenChannelAdapter adapter = new MqttPahoMessageDrivenChannelAdapter(BACKEND_ID + "-inbound",
                mqttClientFactory(), RTOPICS);
        adapter.setOutputChannel(mqttInputChannel());
        adapter.setQos(1);
        monitorConnection(adapter); // Inicia el monitoreo de la conexión
        return adapter;
    }

    // Canal de entrada
    @Bean
    public MessageChannel mqttInputChannel() {
        return new DirectChannel();
    }

    @ServiceActivator(inputChannel = "mqttInputChannel")
    public void handleMqttMessage(@Header("mqtt_receivedTopic") String topic, String payload) {
        System.out.println("Tópico: " + topic + " - Mensaje recibido: " + payload);

        switch (topic) {
            case "Mapping":
                saveMappingValue(payload);
                break;
            case "Messages":
                saveMessage(payload);
                break;
            case "Battery":
                saveBatteryLevel(payload);
                break;
            case "Barrier":
                referencePointService.updateReferencePoint();
                break;

        }
    }

    private void saveMappingValue(String payload) {
        ObjectMapper mapper = new ObjectMapper();
        try {
            MappingValue value = mapper.readValue(payload, MappingValue.class);
            mappingValueService.saveSensorValue(value);
        } catch (JsonProcessingException e) {
            e.printStackTrace(); // Manejo simple de la excepción, puedes mejorar esto
        }

    }

    private void saveMessage(String payload) {
        ObjectMapper mapper = new ObjectMapper();
        try {
            Message value = mapper.readValue(payload, Message.class);
            messageService.saveMessage(value);
        } catch (JsonProcessingException e) {
            e.printStackTrace(); // Manejo simple de la excepción, puedes mejorar esto
        }
    }

    private void saveBatteryLevel(String payload) {
        ObjectMapper mapper = new ObjectMapper();
        try {
            BatteryLevel value = mapper.readValue(payload, BatteryLevel.class);
            batteryLevelService.saveBatteryLevel(value);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
    }

    @PreDestroy
    public void shutdown() {
        running = false;
    }

    // Detecta cambios de conexión y realiza reconexión automática
    private void monitorConnection(MqttPahoMessageDrivenChannelAdapter adapter) {
        new Thread(() -> {
            while (running) {
                try {
                    if (!NetworkUtils.isMyIp(BACKEND_IP)) {
                        System.out.println("IP cambiada o conexión perdida. Intentando reconectar...");
                        waitForBroker();
                        // adapter.removeMessageHandler(this::handleMqttMessage);
                        reconnect(adapter);
                    }
                } catch (Exception e) {
                    System.err.println("Error en el monitoreo de la conexión: " + e.getMessage());
                }
                try {
                    Thread.sleep(RETRY_INTERVAL_MS);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
        }).start();
    }

    private void reconnect(MqttPahoMessageDrivenChannelAdapter adapter) {
        while (running) {
            try {
                adapter.stop();
                adapter.start();
                System.out.println("Reconexión exitosa.");
                break;
            } catch (Exception e) {
                System.err.println("Error durante la reconexión: " + e.getMessage());
                try {
                    Thread.sleep(RETRY_INTERVAL_MS);
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
        }
    }

}
