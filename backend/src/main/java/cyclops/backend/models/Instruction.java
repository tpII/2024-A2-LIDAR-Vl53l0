package cyclops.backend.models;

import java.util.Date;
import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.TimeSeries;

import lombok.Data;

@TimeSeries(collection = "Instruction", timeField = "date")
@Data
public class Instruction {

    @Id
    private String id;
    private String instruction;
    private String parameter1;
    private String parameter2;

    @CreatedDate
    private Date date;

    public Instruction() {}
    
    public Instruction(String instruction, String parameter1, String parameter2) {
        this.instruction = instruction;
        this.parameter1 = parameter1;
        this.parameter2 = parameter2;
    }

    public Instruction(String instruction, String parameter1, String parameter2, Date date) {
        this.instruction = instruction;
        this.parameter1 = parameter1;
        this.parameter2 = parameter2;
        this.date = date;
    }

    public String getInstruction() {
        return instruction;
    }

    public String getParameter1() {
        return parameter1;
    }

    public String getParameter2() {
        return parameter2;
    }

    public Date getDate() {
        return date;
    }

    public void setInstruction(String instruction) {
        this.instruction = instruction;
    }

    public void setParameter1(String parameter1) {
        this.parameter1 = parameter1;
    }

    public void setParameter2(String parameter2) {
        this.parameter2 = parameter2;
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
                ", parameter1='" + parameter1 + '\'' +
                ", parameter2='" + parameter2 + '\'' +
                ", date=" + date +
                '}';
    }

}
