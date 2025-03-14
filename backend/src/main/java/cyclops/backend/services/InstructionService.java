package cyclops.backend.services;

import java.time.LocalDateTime;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.data.mongodb.core.query.Update;
import org.springframework.messaging.Message;
import org.springframework.messaging.MessageChannel;
import org.springframework.messaging.support.MessageBuilder;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

import cyclops.backend.interfacesDAO.InstructionDAO;
import cyclops.backend.models.Instruction;

@Service
public class InstructionService {

    private final InstructionDAO instructionDAO;
    private MessageChannel mqttOutChannel;
    private final MongoTemplate mongoTemplate;

    public InstructionService(InstructionDAO instructionDAO, MongoTemplate mongoTemplate,
            @Qualifier("mqttOutboundChannel") MessageChannel mqttOuChannel) {
        this.instructionDAO = instructionDAO;
        this.mqttOutChannel = mqttOuChannel;
        this.mongoTemplate = mongoTemplate;

    }

    /**
     * Guarda una instrucción en la base de datos y la envía por MQTT.
     * 
     * @param instruction La instrucción a guardar y enviar.
     */
    @Transactional
    public void saveInstruction(Instruction instruction) {
        instructionDAO.save(instruction);
        System.out.println(instruction.toString());
        String payload = convertInstructionToPayload(instruction);
        System.out.println("Payload en JSON: " + payload);
        sendInstruction(instruction);
    }

    /**
     * Envía una instrucción a través del canal MQTT.
     * 
     * @param instruction La instrucción a enviar.
     */
    private void sendInstruction(Instruction instruction) {
        // Crear el mensaje a enviar (payload)
        String payload = convertInstructionToPayload(instruction);
        System.out.println("Inst: " + payload);
        // Crear el mensaje con el tópico adecuado
        Message<String> message = MessageBuilder.withPayload(payload)
                .setHeader("Instruction", "Instruction") // Establecer el tópico
                .build();

        // Enviar el mensaje al canal mqttOutboundChannel
        mqttOutChannel.send(message);
    }

    /**
     * Convierte una instrucción en una cadena JSON.
     * 
     * @param instruction La instrucción a convertir.
     * @return Representación JSON de la instrucción.
     */
    private String convertInstructionToPayload(Instruction instruction) {
        // Convertir la instrucción en formato adecuado para enviar (puede ser JSON, por
        // ejemplo)
        ObjectMapper mapper = new ObjectMapper();
        try {
            return mapper.writeValueAsString(instruction);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
            return "";
        }
    }

    /**
     * Obtiene una instrucción por su ID.
     * 
     * @param id Identificador de la instrucción.
     * @return Un Optional con la instrucción si se encuentra.
     */
    public Optional<Instruction> getInstruction(String id) {
        return instructionDAO.findById(id);
    }

    /**
     * Elimina una instrucción por su ID.
     * 
     * @param id Identificador de la instrucción a eliminar.
     */
    public void deleteInstruction(String id) {
        instructionDAO.deleteById(id);
    }

    /**
     * Obtiene la última instrucción no leída.
     * 
     * @return Un Optional con la última instrucción no leída, si existe.
     */
    public Optional<Instruction> getLastInstruction() {
        Query query = new Query();
        query.addCriteria(Criteria.where("read").is(false));

        query.with(Sort.by(Sort.Direction.DESC, "date"));
        query.limit(1);

        Instruction lastInstruction = mongoTemplate.findOne(query, Instruction.class);

        if (lastInstruction != null) {
            Query updateQuery = new Query(Criteria.where("_id").is(lastInstruction.getId()));
            Update update = new Update().set("read", true);
            mongoTemplate.updateFirst(updateQuery, update, Instruction.class);
        }
        return Optional.ofNullable(lastInstruction);
    }

}
