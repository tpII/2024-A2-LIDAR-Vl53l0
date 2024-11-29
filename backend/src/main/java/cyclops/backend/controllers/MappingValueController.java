package cyclops.backend.controllers;

import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.server.ResponseStatusException;

import cyclops.backend.models.MappingValue;
import cyclops.backend.services.MappingValueService;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;

@RestController
@RequestMapping("/sensor")
public class MappingValueController {
    
    private final MappingValueService sensorValueService;

    public MappingValueController(MappingValueService sensorValueService) {
        this.sensorValueService = sensorValueService;
    }

    @PostMapping
    public void saveMessage(@RequestBody MappingValue sensorValue) {
        sensorValueService.saveSensorValue(sensorValue);
    }
    
    @GetMapping("/{id}")
    public MappingValue getMessage(@PathVariable String id) {
        return sensorValueService.getSensorValue(id)
                .orElseThrow(() -> new ResponseStatusException(HttpStatus.NOT_FOUND, "Message not found with id: " + id));
    }

    @DeleteMapping("/{id}")
    public void deleteMessage(@PathVariable String id) {
        sensorValueService.deleteSensorValue(id);
    }
    
}
