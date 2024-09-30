package cyclops.backend.interfacesDAO;

import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.stereotype.Repository;

import cyclops.backend.models.SensorValue;

@Repository
public interface SensorValueDAO extends  MongoRepository<SensorValue, String> {
    
}
