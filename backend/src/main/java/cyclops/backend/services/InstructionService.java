package cyclops.backend.services;

import java.util.Optional;
import org.springframework.stereotype.Service;

import cyclops.backend.interfacesDAO.InstructionDAO;
import cyclops.backend.models.Instruction;

@Service
public class InstructionService {

    private final InstructionDAO instructionDAO;

    public InstructionService(InstructionDAO instructionDAO) {
        this.instructionDAO = instructionDAO;
    }

    public void saveInstruction(Instruction instruction) {
        instructionDAO.save(instruction);
    }

    public Optional<Instruction> getInstruction(String id) {
        return instructionDAO.findById(id);
    }

    public void deleteInstruction(String id) {
        instructionDAO.deleteById(id);
    }

}
