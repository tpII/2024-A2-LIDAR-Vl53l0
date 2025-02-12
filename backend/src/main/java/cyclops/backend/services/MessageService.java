package cyclops.backend.services;

import java.time.LocalDateTime;
import java.util.Optional;

import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.data.mongodb.core.query.Update;
import org.springframework.stereotype.Service;

import cyclops.backend.interfacesDAO.MessageDAO;
import cyclops.backend.models.Message;

/**
 * Servicio encargado de la gestión de mensajes.
 * Contiene métodos para guardar, obtener, eliminar y actualizar los mensajes en la base de datos.
 * Utiliza el repositorio MessageDAO para las operaciones CRUD y MongoTemplate para consultas personalizadas.
 */
@Service
public class MessageService {

    private final MessageDAO messageDAO;
    private final MongoTemplate mongoTemplate;

     /**
     * Constructor de la clase MessageService.
     * 
     * @param messageDAO El repositorio DAO que maneja los mensajes.
     * @param mongoTemplate El objeto MongoTemplate para realizar consultas avanzadas en MongoDB.
     * @param referencePointService El servicio que maneja los puntos de referencia.
     */
    public MessageService(MessageDAO messageDAO, MongoTemplate mongoTemplate) {
        this.messageDAO = messageDAO;
        this.mongoTemplate = mongoTemplate;
    }
 /**
     * Guarda un nuevo mensaje en la base de datos.
     * 
     * @param message El mensaje a guardar.
     */

    public void saveMessage(Message message) {
        messageDAO.save(message);
    }

     /**
     * Obtiene un mensaje por su identificador.
     * 
     * @param id El identificador del mensaje.
     * @return Un Optional que contiene el mensaje si existe, o está vacío si no se encuentra.
     */

    public Optional<Message> getMessage(String id) {
        return messageDAO.findById(id);
    }

     /**
     * Elimina un mensaje de la base de datos por su identificador.
     * 
     * @param id El identificador del mensaje a eliminar.
     */

    public void deleteMessage(String id) {
        messageDAO.deleteById(id);
    }

     /**
     * Obtiene el último mensaje no leído.
     * 
     * @return Un Optional que contiene el último mensaje no leído, o está vacío si no existe.
     */

    public Optional<Message> getLastMesage() {
        Query query = new Query();
        
            query.addCriteria(Criteria.where("read").is(false));

        query.with(Sort.by(Sort.Direction.DESC, "date"));
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
