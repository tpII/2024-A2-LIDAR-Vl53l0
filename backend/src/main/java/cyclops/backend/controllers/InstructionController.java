package cyclops.backend.controllers;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.server.ResponseStatusException;

import cyclops.backend.models.Instruction;
import cyclops.backend.services.InstructionService;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;



@RestController
@RequestMapping("/instruction")
public class InstructionController {

    private final InstructionService instructionService;

    public InstructionController(InstructionService instructionService) {
        this.instructionService = instructionService;
    }

    @PostMapping
    public void saveInstruction(@RequestBody Instruction instruction) {
        instructionService.saveInstruction(instruction);
    }
    
    @GetMapping("/{id}")
    public Instruction getInstruction(@PathVariable String id) {
        return instructionService.getInstruction(id)
                .orElseThrow(() -> new ResponseStatusException(HttpStatus.NOT_FOUND, "Message not found with id: " + id));
    }
    
    @DeleteMapping("/{id}")
    public void deleteInstruction(@PathVariable String id) {
        instructionService.deleteInstruction(id);
    }

}
