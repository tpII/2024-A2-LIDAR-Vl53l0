package cyclops.backend.services;

import java.time.LocalDateTime;
import java.util.Optional;

import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.data.mongodb.core.query.Update;
import org.springframework.stereotype.Service;
import cyclops.backend.models.BatteryLevel;
import cyclops.backend.interfacesDAO.BatteryLevelDAO;

@Service
public class BatteryLevelService {

    private final BatteryLevelDAO batteryLevelDAO;
    private final MongoTemplate mongoTemplate;
    private final ReferencePointService referencePointService;

    public BatteryLevelService(BatteryLevelDAO batteryLevelDAO, MongoTemplate mongoTemplate,
            ReferencePointService referencePointService) {
        this.batteryLevelDAO = batteryLevelDAO;
        this.mongoTemplate = mongoTemplate;
        this.referencePointService = referencePointService;
    }

    public void saveBatteryLevel(BatteryLevel batteryLevel) {
        batteryLevelDAO.save(batteryLevel);
    }

    public Optional<BatteryLevel> getBatteryLevel(String id) {
        return batteryLevelDAO.findById(id);
    }

    public void deleteBatteryLevel(String id) {
        batteryLevelDAO.deleteById(id);
    }

    public Optional<BatteryLevel> getLastBatteryLevel() {
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

        BatteryLevel batteryLevelInstruction = mongoTemplate.findOne(query, BatteryLevel.class);

        if (batteryLevelInstruction != null) {
            Query updateQuery = new Query(Criteria.where("_id").is(batteryLevelInstruction.getId()));
            Update update = new Update().set("read", true);
            mongoTemplate.updateFirst(updateQuery, update, BatteryLevel.class);
        }
        return Optional.ofNullable(batteryLevelInstruction);
    }

}
