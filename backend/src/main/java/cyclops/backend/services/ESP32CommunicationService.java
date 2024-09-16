package cyclops.backend.services;

import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

import cyclops.backend.models.Message;
import cyclops.backend.models.SensorValue;

@Service
public class ESP32CommunicationService {

    private final String ESP32_IP_ADDRESS = "192.168.1.100";
    private final String ESP32_PORT = "8080";
    private final String ESP32_URL = "http://" + ESP32_IP_ADDRESS + ":" + ESP32_PORT;
    // Revisar de crear bien la URL para que se pueda acceder al ESP32

    private final MessageService messageService;

    public ESP32CommunicationService(MessageService messageService) {
        this.messageService = messageService;
    }

    @Scheduled(fixedRate = 1000) // 1 second
    public void getMessage() {
        RestTemplate restTemplate = new RestTemplate();
        try {
            Message message = restTemplate.getForObject(ESP32_URL + "/message", Message.class);
            if (message != null) {
                messageService.saveMessage(message); 
                System.out.println("Mensaje recibido y guardado: " + message);
            }
        } catch (Exception e) {
            System.out.println("Error al recibir mensaje: " + e.getMessage());
        }
    }
    /*
     * @Scheduled(fixedRate = 1000) // 1 second
     * public void getSensorValue(){
     * RestTemplate restTemplate = new RestTemplate();
     * try {
     * SensorValue sensorValue = restTemplate.getForObject(ESP32_URL +
     * "/sensorValue", SensorValue.class);
     * if (sensorValue != null) {
     * messageService.saveMessage(message); // Asumiendo que tienes un método para
     * guardar mensajes
     * System.out.println("Mensaje recibido y guardado: " + sensorValue);
     * }
     * } catch (Exception e) {
     * System.out.println("Error al recibir mensaje: " + e.getMessage());
     * }
     * }
     */

}
