package devs.alex.sah.rat.c2server.webSocket.user;

import lombok.extern.slf4j.Slf4j;
import org.springframework.http.server.ServerHttpRequest;
import org.springframework.http.server.ServerHttpResponse;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.WebSocketHandler;
import org.springframework.web.socket.server.HandshakeInterceptor;

import java.util.Map;
import java.util.Objects;
import java.util.UUID;

@Slf4j
@Component
public class UserHandshakeInterceptor implements HandshakeInterceptor {


    @Override
    public boolean beforeHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler, Map<String, Object> attributes) throws Exception {

        String authKey, userId;
        try {
            authKey = Objects.requireNonNull(request.getHeaders().get("authentication")).get(0);
        } catch (Exception exception) {
            log.debug("Failed to read \"authentication\" header");
            //return false;
        }
        // TODO manage authentication

        userId = UUID.randomUUID().toString();
        attributes.put("userId", userId);
        response.getHeaders().add("user-id", userId);
        return true;
    }

    @Override
    public void afterHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler, Exception exception){}
}
