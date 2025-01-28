package cyclops.backend.interfacesDAO;

import cyclops.backend.models.BatteryLevel;
import org.springframework.data.mongodb.repository.MongoRepository;

public interface BatteryLevelDAO extends MongoRepository<BatteryLevel, String>{ 
    
}
