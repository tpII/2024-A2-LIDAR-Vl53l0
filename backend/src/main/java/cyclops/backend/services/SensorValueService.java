package cyclops.backend.services;


import java.util.Optional;
import org.springframework.stereotype.Service;
import cyclops.backend.interfacesDAO.SensorValueDAO;
import cyclops.backend.models.SensorValue;

@Service
public class SensorValueService {
    
    private final SensorValueDAO sensorValueDAO;

    public SensorValueService(SensorValueDAO sensorValueDAO) {
        this.sensorValueDAO = sensorValueDAO;
    }

    public void saveSensorValue(SensorValue sensorValue) {
        sensorValueDAO.save(sensorValue);
    }

    public Optional<SensorValue> getSensorValue(String id) {
        return sensorValueDAO.findById(id);
    }

    public void deleteSensorValue(String id) {
        sensorValueDAO.deleteById(id);
    }
    
}
