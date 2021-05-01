package devs.alex.sah.rat.c2server.models;

import lombok.Getter;

@Getter
public class AssociativePair {
    String sessionUUID;
    String requestUUID;

    public AssociativePair(String sessionUUID, String requestUUID) {
        this.sessionUUID = sessionUUID;
        this.requestUUID = requestUUID;
    }

}
