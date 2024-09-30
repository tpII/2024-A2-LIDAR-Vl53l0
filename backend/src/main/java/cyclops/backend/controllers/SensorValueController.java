package cyclops.backend.controllers;

import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.server.ResponseStatusException;

import cyclops.backend.models.SensorValue;
import cyclops.backend.services.SensorValueService;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;

@RestController
@RequestMapping("/sensorvalue")
public class SensorValueController {
    
    private final SensorValueService sensorValueService;

    public SensorValueController(SensorValueService sensorValueService) {
        this.sensorValueService = sensorValueService;
    }

    @PostMapping
    public void saveMessage(@RequestBody SensorValue sensorValue) {
        sensorValueService.saveSensorValue(sensorValue);
    }
    
    @GetMapping("/{id}")
    public SensorValue getMessage(@PathVariable String id) {
        return sensorValueService.getSensorValue(id)
                .orElseThrow(() -> new ResponseStatusException(HttpStatus.NOT_FOUND, "Message not found with id: " + id));
    }

    @DeleteMapping("/delete/{id}")
    public void deleteMessage(@PathVariable String id) {
        sensorValueService.deleteSensorValue(id);
    }
    
}
