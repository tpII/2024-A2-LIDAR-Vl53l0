package cyclops.backend.models;

import java.time.LocalDateTime;
import java.util.Date;

import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.TimeSeries;

import io.swagger.v3.oas.annotations.media.Schema;

//@Document(collection = "messages")
@TimeSeries(collection = "Messages", timeField = "date")
@Schema(description = "Represents a message from MCU")
public class Message {

    @Id
    @Schema(description = "Unique identifier for the message", example = "63f7b9a6e94b1e456d2a3c9f")
    private String id;

    @Schema(description = "The content of the message", example = "System maintenance scheduled for tomorrow")
    private String message;

    @Schema(description = "The type of the message", example = "Notification")
    private String type;

    @Schema(description = "An optional tag associated with the message for categorization", example = "system")
    private String tag;

    @Schema(description = "Indicates whether the message has been read", example = "false")
    private boolean read = false;

    @CreatedDate
    @Schema(description = "The date and time when the message was created", example = "2024-12-05T14:30:00Z", format = "date-time")
    private LocalDateTime date;

    public Message() {
    }

    public Message(String message) {
        this.message = message;
    }

    public Message(String message, LocalDateTime date) {
        this.message = message;
        this.date = date;
    }

    public String getMessage() {
        return message;
    }

    public LocalDateTime getDate() {
        return date;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public void setDate(LocalDateTime date) {
        this.date = date;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getType() {
        return type;
    }

    public void setTag(String tag) {
        this.tag = tag;
    }

    public String getTag() {
        return tag;
    }

    public void setRead(boolean read){
        this.read = read;
    }

    public boolean getRead(){
        return read;
    }

    @Override
    public String toString() {
        return "Message{" +
                "id='" + id + '\'' + ", tag: " + tag + ", type: " + type +
                ", message='" + message + '\'' +
                ", date=" + date +
                '}';
    }
}