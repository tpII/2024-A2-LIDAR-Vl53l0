package cyclops.backend.interfacesDAO;

import org.springframework.stereotype.Repository;

import cyclops.backend.models.Instruction;
import org.springframework.data.mongodb.repository.MongoRepository;

@Repository
public interface InstructionDAO extends MongoRepository<Instruction, String>{
    
}
