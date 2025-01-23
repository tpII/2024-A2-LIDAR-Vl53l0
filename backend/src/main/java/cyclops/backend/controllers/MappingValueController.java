package cyclops.backend.controllers;

import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.server.ResponseStatusException;

import cyclops.backend.models.MappingValue;
import cyclops.backend.services.MappingValueService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

import java.util.List;

import org.springframework.http.HttpStatus;
import org.springframework.http.HttpStatusCode;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestParam;

@RestController
@RequestMapping("/sensor")
@Tag(name = "Sensor Controller", description = "Handles operations related to sensor mapping values")
public class MappingValueController {

    private final MappingValueService sensorValueService;

    public MappingValueController(MappingValueService sensorValueService) {
        this.sensorValueService = sensorValueService;
    }

    @PostMapping
    @Operation(summary = "Save a new sensor mapping value", description = "Stores a new mapping value in the database.")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Mapping value saved successfully"),
            @ApiResponse(responseCode = "400", description = "Invalid request body", content = @Content)
    })
    public void saveMessage(@RequestBody MappingValue sensorValue) {
        sensorValueService.saveSensorValue(sensorValue);
    }

    @GetMapping("/{id}")
    @Operation(summary = "Retrieve a mapping value by ID", description = "Fetches a specific mapping value using its unique identifier.")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Mapping value retrieved successfully", content = @Content(schema = @Schema(implementation = MappingValue.class))),
            @ApiResponse(responseCode = "404", description = "Mapping value not found", content = @Content)
    })
    public MappingValue getMessage(@PathVariable String id) {
        return sensorValueService.getSensorValue(id)
                .orElseThrow(
                        () -> new ResponseStatusException(HttpStatus.NOT_FOUND, "Message not found with id: " + id));
    }

    @DeleteMapping("/{id}")
    @Operation(summary = "Delete a mapping value by ID", description = "Deletes a mapping value using its unique identifier.")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Mapping value deleted successfully"),
            @ApiResponse(responseCode = "404", description = "Mapping value not found", content = @Content)
    })
    public void deleteMessage(@PathVariable String id) {
        sensorValueService.deleteSensorValue(id);
    }

    @GetMapping("/values")
    @Operation(summary = "Retrieve all unread mapping values", description = "Retrieve a list of unread mapping values.")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Mapping value list retrieved successfully", content = @Content(schema = @Schema(implementation = MappingValue.class))),
            @ApiResponse(responseCode = "404", description = "Mapping value not found", content = @Content)
    })
    public ResponseEntity<?> getUnreadValues() {
        List<MappingValue> unreadValues = sensorValueService.getUnreadMappingValues();
        if (unreadValues.isEmpty()) {
            return ResponseEntity.status(HttpStatus.NOT_FOUND).body("No se encontraron valores.");
        }
        return ResponseEntity.ok(unreadValues);
    }

}
