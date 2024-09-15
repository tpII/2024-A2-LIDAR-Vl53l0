package cyclops.backend.models;

import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.TimeSeries;

//@Document(collection = "sensorvalues")
@TimeSeries(collection = "sensorvalues", timeField = "date")
public class SensorValue {

    @Id
    private String id;
    private String distance;
    private String angle;
    
    @CreatedDate
    private String date;

    public SensorValue(String distance, String angle, String date) {
        this.distance = distance;
        this.angle = angle;
        this.date = date;
    }

    public String getDistance() {
        return distance;
    }

    public String getAngle() {
        return angle;
    }

    public String getDate() {
        return date;
    }

    public void setDistance(String distance) {
        this.distance = distance;
    }

    public void setAngle(String angle) {
        this.angle = angle;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }
}
