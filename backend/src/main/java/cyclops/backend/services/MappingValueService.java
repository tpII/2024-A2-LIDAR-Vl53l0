package cyclops.backend.services;


import java.util.Optional;
import org.springframework.stereotype.Service;
import cyclops.backend.interfacesDAO.MappingValueDAO;
import cyclops.backend.models.MappingValue;

@Service
public class MappingValueService {
    
    private final MappingValueDAO sensorValueDAO;

    public MappingValueService(MappingValueDAO sensorValueDAO) {
        this.sensorValueDAO = sensorValueDAO;
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
    
}
