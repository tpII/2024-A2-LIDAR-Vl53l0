package cyclops.backend.models;

import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.mapping.TimeSeries;

//@Document(collection = "sensorvalues")
@TimeSeries(collection = "MappingValues", timeField = "date")
public class MappingValue {

    @Id
    private String id;
    private long distance;
    private int angle;
    
    @CreatedDate
    private String date;

    public MappingValue(){}

    public MappingValue(long distance, int angle, String date) {
        this.distance = distance;
        this.angle = angle;
        this.date = date;
    }

    public long getDistance() {
        return distance;
    }

    public int getAngle() {
        return angle;
    }

    public String getDate() {
        return date;
    }

    public void setDistance(long distance) {
        this.distance = distance;
    }

    public void setAngle(int angle) {
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
