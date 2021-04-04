package devs.alex.sah.rat.c2server.models;


import java.util.Map;

public class Message<T> {

    private Map<String, String> envelopeParams;
    private T payload;

    public Message(Map<String, String> envelopeParams, T payload) {
        this.envelopeParams = envelopeParams;
        this.payload = payload;
    }


    public Map<String, String> params() {
        return envelopeParams;
    }


    public T getPayload() {
        return payload;
    }

    public void setPayload(T payload) {
        this.payload = payload;
    }
}
