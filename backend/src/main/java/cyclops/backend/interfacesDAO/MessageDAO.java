package cyclops.backend.interfacesDAO;

import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.stereotype.Repository;

import cyclops.backend.models.Message;

@Repository
public interface MessageDAO extends  MongoRepository<Message, String> {



}