package cyclops.backend.services;

import java.util.Optional;

import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.data.mongodb.core.query.Update;
import org.springframework.stereotype.Service;

import cyclops.backend.interfacesDAO.MessageDAO;
import cyclops.backend.models.Message;

@Service
public class MessageService {

    private final MessageDAO messageDAO;
    private final MongoTemplate mongoTemplate;

    public MessageService(MessageDAO messageDAO, MongoTemplate mongoTemplate) {
        this.messageDAO = messageDAO;
        this.mongoTemplate = mongoTemplate;
    }

    public void saveMessage(Message message) {
        messageDAO.save(message);
    }

    public Optional<Message> getMessage(String id) {
        return messageDAO.findById(id);
    }

    public void deleteMessage(String id) {
        messageDAO.deleteById(id);
    }
    
    public Optional<Message> getLastMesage(){
        Query query = new Query();
        query.addCriteria(Criteria.where("read").is(false));
        query.with(Sort.by(Sort.Direction.DESC,"date"));
        query.limit(1);

        Message lastMessage = mongoTemplate.findOne(query, Message.class);

        if (lastMessage != null) {
            Query updateQuery = new Query(Criteria.where("_id").is(lastMessage.getId()));
            Update update = new Update().set("read", true);
            mongoTemplate.updateFirst(updateQuery, update, Message.class);
        }
    
        return Optional.ofNullable(lastMessage);

    }
   
}
