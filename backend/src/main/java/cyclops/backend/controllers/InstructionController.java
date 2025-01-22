package cyclops.backend.controllers;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.server.ResponseStatusException;

import cyclops.backend.models.Instruction;
import cyclops.backend.services.InstructionService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;

@RestController
@RequestMapping("/instruction")
@Tag(name = "Instruction Controller", description = "Handles operations related to instructions")
public class InstructionController {

    private final InstructionService instructionService;

    public InstructionController(InstructionService instructionService) {
        this.instructionService = instructionService;
    }

    @PostMapping
    @Operation(summary = "Save a new instruction", description = "Stores a new instruction in the database")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Instruction saved successfully"),
            @ApiResponse(responseCode = "400", description = "Invalid request body", content = @Content)
    })
    public void saveInstruction(@RequestBody Instruction instruction) {
        instructionService.saveInstruction(instruction);
    }

    @GetMapping("/{id}")
    @Operation(summary = "Get an instruction by ID", description = "Retrieves an instruction using its unique identifier")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Instruction retrieved successfully", content = @Content(schema = @Schema(implementation = Instruction.class))),
            @ApiResponse(responseCode = "404", description = "Instruction not found", content = @Content)
    })
    public Instruction getInstruction(@PathVariable String id) {
        return instructionService.getInstruction(id)
                .orElseThrow(
                        () -> new ResponseStatusException(HttpStatus.NOT_FOUND, "Message not found with id: " + id));
    }

    @DeleteMapping("/{id}")
    @Operation(summary = "Delete an instruction by ID", description = "Deletes an instruction using its unique identifier")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Instruction deleted successfully"),
            @ApiResponse(responseCode = "404", description = "Instruction not found", content = @Content)
    })
    public void deleteInstruction(@PathVariable String id) {
        instructionService.deleteInstruction(id);
    }

    @GetMapping("/last")
    @Operation(summary = "Retrieve the last instruction", description = "Fetches the most recent instruction from the database.")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "Last intruction retrieved successfully", content = @Content(schema = @Schema(implementation = Instruction.class))),
            @ApiResponse(responseCode = "204", description = "No new instruction available", content = @Content)
    })
    public Instruction getLaInstruction() {
        return instructionService.getLastInstruction()
                .orElseThrow(() -> new ResponseStatusException(HttpStatus.NO_CONTENT, "NO NEW INSTRUCTION"));
    }
}
