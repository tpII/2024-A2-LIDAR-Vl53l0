package cyclops.backend.models;

import java.time.LocalDateTime;

import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.TimeSeries;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;

@TimeSeries(collection = "Instruction", timeField = "time")
@Schema(description = "Model representing the battery level reported to the system")
public class BatteryLevel {

    @Id
    @Schema(description = "Unique identifier of the instruction", example = "63f7b9a6e94b1e456d2a3c9f")
    private String id;

    @NotBlank(message = "Instruction description cannot be blank")
    @Schema(description = "Description of the instruction", example = "Forward")
    private int level;

    @CreatedDate
    @Schema(description = "Date and time the battery level was created", example = "2024-12-05T14:30:00Z", format = "date-time")
    private LocalDateTime time;

    @Schema(description = "Indicates whether the battery level has been read", example = "false")
    private boolean read = false;

    public BatteryLevel(){}

    public BatteryLevel(int level){
        this.level=level;
    }

    public BatteryLevel(int level, LocalDateTime time){
        this.level=level;
        this.time=time;
    }

    public void setLevel(int level){
        this.level=level;
    }

    public int getLevel(){
        return level;
    }

    public LocalDateTime getTime() {
        return time;
    }

    public void setTime(LocalDateTime time) {
        this.time = time;
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

    

}
