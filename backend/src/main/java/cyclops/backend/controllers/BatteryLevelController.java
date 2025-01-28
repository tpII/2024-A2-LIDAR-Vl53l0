package cyclops.backend.controllers;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.server.ResponseStatusException;

import cyclops.backend.models.BatteryLevel;
import cyclops.backend.services.BatteryLevelService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;

@RestController
@RequestMapping("/battery-level")
@Tag(name = "BatteryLevel Controller", description = "Handles operations related to bettery level")
public class BatteryLevelController {

    private final BatteryLevelService batteryLevelService;

    public BatteryLevelController(BatteryLevelService batteryLevelService) {
        this.batteryLevelService = batteryLevelService;
    }

    @GetMapping("/last")
    @Operation(summary = "Retrieve the battery level", description = "Fetches the most recent battery level from the database.")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Last battery level retrieved successfully", content = @Content(schema = @Schema(implementation = BatteryLevel.class))),
            @ApiResponse(responseCode = "204", description = "No new battery level available", content = @Content)
    })
    public BatteryLevel getLaInstruction() {
        return batteryLevelService.getLastBatteryLevel()
                .orElseThrow(() -> new ResponseStatusException(HttpStatus.NO_CONTENT, "NO NEW INSTRUCTION"));

    }
}
