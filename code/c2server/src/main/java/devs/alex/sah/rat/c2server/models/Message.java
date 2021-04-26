package devs.alex.sah.rat.c2server.models;


import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Map;

@Data
@NoArgsConstructor
public class Message<T> {

    //private Map<String, String> envelopeParams;

    private String packageType;
    private String recipientType;
    private String recipientID;
    private String module;
    private String requestID;
    private String responseType;
    private String fullMessageSize;
    private String isLast;

    private T payload;

    public Message(Map<String, String> envelopeParams, T payload) {
        //this.envelopeParams = envelopeParams;
        this.payload = payload;
    }
    public Message(T payload) {
        this.payload = payload;
    }


    public T payload() {
        return payload;
    }

    public void setPayload(T payload) {
        this.payload = payload;
    }
}
