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

    private static final String[] serverUri = { "tcp://localhost:1883" };
    private static final String[] TOPICS = { "Mapping", "Messages", "Battery", "Instruction" };
    private final MappingValueService mappingValueService;
    private final MessageService messageService;

    public MqttConfig(MappingValueService mappingValueService, MessageService messageService) {
        this.mappingValueService = mappingValueService;
        this.messageService = messageService;
    }

    @Bean
    public MqttPahoClientFactory mqttClientFactory() {
        DefaultMqttPahoClientFactory factory = new DefaultMqttPahoClientFactory();
        MqttConnectOptions options = new MqttConnectOptions();
        options.setServerURIs(serverUri); // Broker MQTT
        factory.setConnectionOptions(options);
        return factory;
    }

    @Bean
    public MessageChannel mqttOutboundChannel() {
        return new DirectChannel();
    }

    @Bean
    @ServiceActivator(inputChannel = "mqttOutboundChannel")
    public MqttPahoMessageHandler mqttMessageHandler() {
        MqttPahoMessageHandler messageHandler = new MqttPahoMessageHandler("clientId-outbound", mqttClientFactory());
        messageHandler.setDefaultTopic(TOPICS[3]);
        return messageHandler;
    }

    // Escucha de mensajes entrantes en los tópicos
    @Bean
    public MessageProducer inbound() {
        MqttPahoMessageDrivenChannelAdapter adapter = new MqttPahoMessageDrivenChannelAdapter("clientId-inbound",
                mqttClientFactory(), TOPICS);
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
