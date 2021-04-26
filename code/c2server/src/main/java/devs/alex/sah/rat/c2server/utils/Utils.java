package devs.alex.sah.rat.c2server.utils;

import org.springframework.web.socket.WebSocketSession;
import java.util.*;

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

    private static Set<String> findDuplicates(List<String> toFind) {
        final Set<String> duplicates = new HashSet<>();
        final Set<String> set1 = new HashSet<>();
        for (String el : toFind){
            if(!set1.add(el)) {
                duplicates.add(el);
            }
        }
        return duplicates;
    }

    public static String lengthCheck(String result){
        if(result.length() <= 97){
            return result.length()+2 + result;
        } else{
            return result.length()+3 + result;
        }
    }

}
