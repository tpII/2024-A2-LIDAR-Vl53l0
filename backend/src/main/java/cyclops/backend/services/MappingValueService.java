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

/**
 * Servicio encargado de la gestión de los valores de mapeo (MappingValue).
 * Contiene métodos para guardar, obtener, eliminar y actualizar los valores de
 * mapeo en la base de datos.
 * 
 * Utiliza el repositorio MappingValueDAO para las operaciones CRUD y
 * MongoTemplate para consultas personalizadas.
 */
@Service
public class MappingValueService {

    private final MappingValueDAO sensorValueDAO;
    private final MongoTemplate mongoTemplate;

    /**
     * Constructor de la clase MappingValueService.
     * 
     * @param sensorValueDAO El repositorio DAO que maneja los valores de mapeo.
     * @param mongoTemplate  El objeto MongoTemplate para realizar consultas
     *                       avanzadas en MongoDB.
     */
    public MappingValueService(MappingValueDAO sensorValueDAO, MongoTemplate mongoTemplate) {
        this.sensorValueDAO = sensorValueDAO;
        this.mongoTemplate = mongoTemplate;

    }

    /**
     * Guarda un nuevo valor de mapeo en la base de datos.
     * 
     * @param sensorValue El valor de mapeo a guardar.
     */
    public void saveSensorValue(MappingValue sensorValue) {
        sensorValueDAO.save(sensorValue);
    }

    /**
     * Obtiene un valor de mapeo por su identificador.
     * 
     * @param id El identificador del valor de mapeo.
     * @return Un Optional que contiene el valor de mapeo si existe, o está vacío si
     *         no se encuentra.
     */
    public Optional<MappingValue> getSensorValue(String id) {
        return sensorValueDAO.findById(id);
    }

    /**
     * Elimina un valor de mapeo de la base de datos por su identificador.
     * 
     * @param id El identificador del valor de mapeo a eliminar.
     */
    public void deleteSensorValue(String id) {
        sensorValueDAO.deleteById(id);
    }

    /**
     * Obtiene una lista de valores de mapeo que no han sido leídos (propiedad
     * "read" en falso).
     * Marca todos los valores obtenidos como leídos (cambia la propiedad "read" a
     * verdadero).
     * 
     * @return Lista de valores de mapeo que no han sido leídos.
     */
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
