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
    private final ReferencePointService referencePointService;
    private final MongoTemplate mongoTemplate;

    public InstructionService(InstructionDAO instructionDAO, MongoTemplate mongoTemplate,
            @Qualifier("mqttOutboundChannel") MessageChannel mqttOuChannel, ReferencePointService referencePointService) {
        this.instructionDAO = instructionDAO;
        this.mqttOutChannel = mqttOuChannel;
        this.mongoTemplate = mongoTemplate;
        this.referencePointService = referencePointService;

    }

    @Transactional
    public void saveInstruction(Instruction instruction) {
        instructionDAO.save(instruction);
        System.out.println(instruction.toString());
        String payload = convertInstructionToPayload(instruction);
        System.out.println("Payload en JSON: " + payload);
        sendInstruction(instruction);
    }

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

    public Optional<Instruction> getInstruction(String id) {
        return instructionDAO.findById(id);
    }

    public void deleteInstruction(String id) {
        instructionDAO.deleteById(id);
    }

    public Optional<Instruction> getLastInstruction() {
        LocalDateTime rp = referencePointService.getReferencePoint()
                .orElseThrow(() -> new IllegalStateException("Punto de referencia no establecido."));
        Query query = new Query();
        if (rp == null) {
            query.addCriteria(Criteria.where("read").is(false));
        } else {
            query.addCriteria(Criteria.where("read").is(false).and("date")
                    .gte(rp));
        }
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
