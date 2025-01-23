package cyclops.backend.services;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Optional;

import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.stereotype.Service;
import cyclops.backend.interfacesDAO.MappingValueDAO;
import cyclops.backend.models.MappingValue;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.data.mongodb.core.query.Update;

@Service
public class MappingValueService {

    private final MappingValueDAO sensorValueDAO;
    private final MongoTemplate mongoTemplate;
    private static final LocalDateTime systemStartTime = LocalDateTime.now();

    public MappingValueService(MappingValueDAO sensorValueDAO, MongoTemplate mongoTemplate) {
        this.sensorValueDAO = sensorValueDAO;
        this.mongoTemplate = mongoTemplate;

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
        Query query = new Query();
        query.addCriteria(Criteria.where("read").is(false).and("date")
                .gte(systemStartTime));

        List<MappingValue> unreadValues = mongoTemplate.find(query, MappingValue.class);
        if (!unreadValues.isEmpty()) {
            Query updateQuery = new Query(Criteria.where("read").is(false).and("date")
                    .gte(systemStartTime));
            Update update = new Update().set("read", true);
            mongoTemplate.updateMulti(updateQuery, update, MappingValue.class);
        }

        return unreadValues;
    }

}
