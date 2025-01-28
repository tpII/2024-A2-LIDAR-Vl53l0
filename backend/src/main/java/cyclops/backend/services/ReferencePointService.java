package cyclops.backend.services;

import java.time.LocalDateTime;
import java.util.Optional;

import org.springframework.stereotype.Service;

@Service
public class ReferencePointService {

    private volatile LocalDateTime referencePoint;

    public synchronized void updateReferencePoint(){
        this.referencePoint=LocalDateTime.now();
        System.out.println("Punto de referencia actualizado a: "+this.referencePoint);
    }

    public Optional<LocalDateTime> getReferencePoint(){
        return Optional.ofNullable(referencePoint);
    }
    
}
