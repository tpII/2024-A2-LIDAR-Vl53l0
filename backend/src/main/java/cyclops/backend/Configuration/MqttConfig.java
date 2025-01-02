package cyclops.backend.Configuration;

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

import cyclops.backend.models.MappingValue;
import cyclops.backend.models.Message;
import cyclops.backend.services.MappingValueService;
import cyclops.backend.services.MessageService;

@Configuration
public class MqttConfig {
    //"tcp://192.168.4.2:1883"
    private static final String BACKEND_IP = "192.168.4.2";
    private static final String[] serverUri = {"tcp://" + BACKEND_IP + ":1883"};
    private static final String[] RTOPICS = { "Mapping", "Messages", "Battery"};
    private static final String[] STOPICS = {"Instruction"};
    private static final String BACKEND_ID = "backend-service"; // ID único para el backend
    private static final int RETRY_INTERVAL_MS = 5000; // Tiempo entre intentos en milisegundos

    private final MappingValueService mappingValueService;
    private final MessageService messageService;

    public MqttConfig(MappingValueService mappingValueService, MessageService messageService) {
        this.mappingValueService = mappingValueService;
        this.messageService = messageService;

    }

    @Bean
    public MqttPahoClientFactory mqttClientFactory() {
        waitForBroker();
        DefaultMqttPahoClientFactory factory = new DefaultMqttPahoClientFactory();
        MqttConnectOptions options = new MqttConnectOptions();
        options.setServerURIs(serverUri); // Broker MQTT
        options.setCleanSession(true);
        factory.setConnectionOptions(options);
        return factory;
    }

    private void waitForBroker() {
        System.out.println("Esperando a que la IP del host sea " + BACKEND_IP + "...");
        while (!NetworkUtils.isMyIp(BACKEND_IP)) {
            try {
                System.out.println("La IP actual no es " + BACKEND_IP + ", reintentando en " + RETRY_INTERVAL_MS + "ms...");
                Thread.sleep(RETRY_INTERVAL_MS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                throw new IllegalStateException("Interrupción durante la espera de la IP correcta", e);
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
        MqttPahoMessageHandler messageHandler = new MqttPahoMessageHandler(BACKEND_ID+"-outbound", mqttClientFactory());
        messageHandler.setDefaultTopic(STOPICS[0]);
        return messageHandler;
    }

    // Escucha de mensajes entrantes en los tópicos
    @Bean
    public MessageProducer inbound() {
        MqttPahoMessageDrivenChannelAdapter adapter = new MqttPahoMessageDrivenChannelAdapter(BACKEND_ID+"-inbound",
                mqttClientFactory(), RTOPICS);
        adapter.setOutputChannel(mqttInputChannel());
        adapter.setQos(1);
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
                // saveBatteryLevel(payload);
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
}
