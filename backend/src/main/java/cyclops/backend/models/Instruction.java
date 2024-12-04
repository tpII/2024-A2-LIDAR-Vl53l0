package cyclops.backend.models;

import java.util.Date;
import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.TimeSeries;

@TimeSeries(collection = "Instruction", timeField = "date")
public class Instruction {

    @Id
    private String id;
    private String instruction;

    @CreatedDate
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
