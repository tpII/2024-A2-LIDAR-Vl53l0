package cyclops.backend.services;

import java.util.Optional;

import org.springframework.stereotype.Service;

import cyclops.backend.interfacesDAO.MessageDAO;
import cyclops.backend.models.Message;

@Service
public class MessageService {

    private final MessageDAO messageDAO;

    public MessageService(MessageDAO messageDAO) {
        this.messageDAO = messageDAO;
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
    
}
