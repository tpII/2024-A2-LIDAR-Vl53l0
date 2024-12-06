package cyclops.backend.models;

import java.util.Date;
import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.TimeSeries;

import io.swagger.v3.oas.annotations.media.Schema;

@TimeSeries(collection = "Instruction", timeField = "date")
@Schema(description = "Model representing an instruction sent to the system")
public class Instruction {

    @Id
    @Schema(description = "Unique identifier of the instruction", example = "63f7b9a6e94b1e456d2a3c9f")
    private String id;

    @Schema(description = "Description of the instruction", example = "Forward")
    private String instruction;

    @CreatedDate
    @Schema(description = "Date and time the instruction was created", example = "2024-12-05T14:30:00Z", format = "date-time")
    private Date date;

    public Instruction() {}
    
    public Instruction(String instruction, String parameter1, String parameter2) {
        this.instruction = instruction;

    }

    public Instruction(String instruction, String parameter1, String parameter2, Date date) {
        this.instruction = instruction;
        this.date = date;
    }

    public String getInstruction() {
        return instruction;
    }

    public Date getDate() {
        return date;
    }

    public void setInstruction(String instruction) {
        this.instruction = instruction;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    @Override
    public String toString() {
        return "Instruction{" +
                "id='" + id + '\'' +
                ", instruction='" + instruction + '\'' +
                ", date=" + date +
                '}';
    }

}
