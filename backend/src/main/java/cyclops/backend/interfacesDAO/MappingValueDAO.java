package cyclops.backend.interfacesDAO;

import org.springframework.data.mongodb.repository.MongoRepository;
import org.springframework.stereotype.Repository;

import cyclops.backend.models.MappingValue;

@Repository
public interface MappingValueDAO extends  MongoRepository<MappingValue, String> {
    
}
