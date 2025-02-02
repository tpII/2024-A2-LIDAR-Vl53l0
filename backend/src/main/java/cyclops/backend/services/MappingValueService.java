package cyclops.backend.services;

import java.util.List;
import java.util.Optional;

import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.data.mongodb.core.query.Update;
import org.springframework.stereotype.Service;

import cyclops.backend.interfacesDAO.MappingValueDAO;
import cyclops.backend.models.MappingValue;

@Service
public class MappingValueService {

    private final MappingValueDAO sensorValueDAO;
    private final MongoTemplate mongoTemplate;
    private final ReferencePointService referencePointService;

    public MappingValueService(MappingValueDAO sensorValueDAO, MongoTemplate mongoTemplate,
            ReferencePointService referencePointService) {
        this.sensorValueDAO = sensorValueDAO;
        this.mongoTemplate = mongoTemplate;
        this.referencePointService = referencePointService;

    }

    public void saveSensorValue(MappingValue sensorValue) {
        sensorValueDAO.save(sensorValue);
    }

    public Optional<MappingValue> getSensorValue(String id) {
        return sensorValueDAO.findById(id);
    }

    public void deleteSensorValue(String id) {
        sensorValueDAO.deleteById(id);
    }

    public List<MappingValue> getUnreadMappingValues() {
        Query query = new Query().addCriteria(Criteria.where("read").is(false));
    
       
        List<MappingValue> unreadValues = mongoTemplate.find(query, MappingValue.class);
        System.out.println("List: " + unreadValues);
    
        if (!unreadValues.isEmpty()) {
            Update update = new Update().set("read", true);
            mongoTemplate.updateMulti(query, update, MappingValue.class);
        }
    
        return unreadValues;
    }
    

}
