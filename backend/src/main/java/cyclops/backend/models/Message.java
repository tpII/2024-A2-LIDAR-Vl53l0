package cyclops.backend.models;

import java.util.Date;

import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.TimeSeries;


//@Document(collection = "messages")
@TimeSeries(collection = "messages", timeField = "date")
public class Message {

    @Id
    private String id;
    private String message;
   
    @CreatedDate
    private Date date;

    public Message() {}
    
    public Message(String message) {
        this.message = message;
    }
    public Message(String message, Date date) {
        this.message = message;
        this.date = date;
    }

    public String getMessage() {
        return message;
    }

    public Date getDate() {
        return date;
    }

    public void setMessage(String message) {
        this.message = message;
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
        return "Message{" +
                "id='" + id + '\'' +
                ", message='" + message + '\'' +
                ", date=" + date +
                '}';
    }
}