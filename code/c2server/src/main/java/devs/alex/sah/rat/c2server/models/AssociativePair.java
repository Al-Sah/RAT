package devs.alex.sah.rat.c2server.models;

import lombok.Getter;
import lombok.NoArgsConstructor;

@Getter
@NoArgsConstructor
public class AssociativePair {
    String senderSession;
    String requestID;
    String targetModule;

    public AssociativePair(String senderSession, String requestID, String targetModule) {
        this.senderSession = senderSession;
        this.requestID = requestID;
        this.targetModule = targetModule;
    }

}
