package cyclops.backend.services;

import java.time.LocalDateTime;
import java.util.Optional;

import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.data.mongodb.core.query.Update;
import org.springframework.stereotype.Service;

import cyclops.backend.interfacesDAO.BatteryLevelDAO;
import cyclops.backend.models.BatteryLevel;

/**
 * Servicio para gestionar los niveles de batería en la base de datos MongoDB.
 */
@Service
public class BatteryLevelService {

    private final BatteryLevelDAO batteryLevelDAO;
    private final MongoTemplate mongoTemplate;

    /**
     * Constructor que inyecta las dependencias necesarias.
     *
     * @param batteryLevelDAO DAO para la gestión de BatteryLevel.
     * @param mongoTemplate   Plantilla de MongoDB para consultas avanzadas.
     */
    public BatteryLevelService(BatteryLevelDAO batteryLevelDAO, MongoTemplate mongoTemplate) {
        this.batteryLevelDAO = batteryLevelDAO;
        this.mongoTemplate = mongoTemplate;
    }

    /**
     * Guarda un nuevo nivel de batería en la base de datos.
     *
     * @param batteryLevel Objeto BatteryLevel a guardar.
     */
    public void saveBatteryLevel(BatteryLevel batteryLevel) {
        batteryLevelDAO.save(batteryLevel);
    }

    /**
     * Obtiene un nivel de batería por su ID.
     *
     * @param id Identificador del nivel de batería.
     * @return Un Optional con el BatteryLevel si se encuentra.
     */
    public Optional<BatteryLevel> getBatteryLevel(String id) {
        return batteryLevelDAO.findById(id);
    }

    /**
     * Elimina un nivel de batería por su ID.
     *
     * @param id Identificador del nivel de batería a eliminar.
     */
    public void deleteBatteryLevel(String id) {
        batteryLevelDAO.deleteById(id);
    }

    /**
     * Obtiene el último nivel de batería no leído.
     * Marca el nivel de batería recuperado como "leído".
     *
     * @return Un Optional con el último BatteryLevel no leído.
     */
    public Optional<BatteryLevel> getLastBatteryLevel() {

        Query query = new Query();
        query.addCriteria(Criteria.where("read").is(false));

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
