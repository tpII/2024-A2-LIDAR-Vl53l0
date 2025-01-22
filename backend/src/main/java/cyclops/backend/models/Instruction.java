package cyclops.backend.models;

import java.time.LocalDateTime;
import java.util.Date;
import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.TimeSeries;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;

@TimeSeries(collection = "Instruction", timeField = "time")
@Schema(description = "Model representing an instruction sent to the system")
public class Instruction {

    @Id
    @Schema(description = "Unique identifier of the instruction", example = "63f7b9a6e94b1e456d2a3c9f")
    private String id;

    @NotBlank(message = "Instruction description cannot be blank")
    @Schema(description = "Description of the instruction", example = "Forward")
    private String instruction;

    @CreatedDate
    @Schema(description = "Date and time the instruction was created", example = "2024-12-05T14:30:00Z", format = "date-time")
    private LocalDateTime time;

    @Schema(description = "Indicates whether the instruction has been read", example = "false")
    private boolean read = false;

    public LocalDateTime getTime() {
        return time;
    }

    public void setTime(LocalDateTime time) {
        this.time = time;
    }

    public Instruction() {
    }

    public Instruction(String instruction, String parameter1, String parameter2) {
        this.instruction = instruction;

    }

    public Instruction(String instruction, String parameter1, String parameter2, LocalDateTime time) {
        this.instruction = instruction;
        this.time = time;
    }

    public String getInstruction() {
        return instruction;
    }

    public void setInstruction(String instruction) {
        this.instruction = instruction;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public void setRead(boolean read) {
        this.read = read;
    }

    public boolean getRead() {
        return read;
    }

    @Override
    public String toString() {
        return "Instruction{" +
                "id='" + id + '\'' +
                ", instruction='" + instruction + '\'' +
                ", date=" + time +
                '}';
    }

}
