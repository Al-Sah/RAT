package devs.alex.sah.rat.c2server;

import devs.alex.sah.rat.c2server.configuration.MessagesConfiguration;
import devs.alex.sah.rat.c2server.services.MessagesBuilder;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import static org.assertj.core.api.Assertions.assertThat;


@SpringBootTest
class ServerApplicationTests {

    @Autowired
    private MessagesBuilder messagesBuilder;
    @Autowired
    private MessagesConfiguration mConfig;

    @Test
    void generateFromUserToBotEnvelopes() throws Exception{
        String res = messagesBuilder.generateUser2BotFirstEnvelope("test", "association", "single", "1024");
        assertThat(res).containsSubsequence(mConfig.keys.packageType + mConfig.delimiters.value + mConfig.packages.firstPart);
        assertThat(res).containsSubsequence(mConfig.keys.targetModule + mConfig.delimiters.value + "test");
        assertThat(res).containsSubsequence(mConfig.keys.requestID + mConfig.delimiters.value + "association");
        assertThat(res).containsSubsequence(mConfig.keys.responseType + mConfig.delimiters.value + "single");
        assertThat(res).containsSubsequence(mConfig.keys.fullMessageSize + mConfig.delimiters.value + "1024");

        res = messagesBuilder.generateUser2BotSingleEnvelope("test","association", "many");
        assertThat(res).containsSubsequence(mConfig.keys.packageType + mConfig.delimiters.value + mConfig.packages.singleMessage);
        assertThat(res).containsSubsequence(mConfig.keys.targetModule + mConfig.delimiters.value + "test");
        assertThat(res).containsSubsequence(mConfig.keys.requestID + mConfig.delimiters.value + "association");
        assertThat(res).containsSubsequence(mConfig.keys.responseType + mConfig.delimiters.value + "many");
        assertThat(res).doesNotContain(mConfig.keys.fullMessageSize);

        res = messagesBuilder.generateUser2BotEnvelope("test", "association", mConfig.packages.continuation);
        assertThat(res).containsSubsequence(mConfig.keys.packageType + mConfig.delimiters.value + mConfig.packages.continuation);
        assertThat(res).containsSubsequence(mConfig.keys.targetModule + mConfig.delimiters.value + "test");
        assertThat(res).containsSubsequence(mConfig.keys.requestID + mConfig.delimiters.value + "association");
        assertThat(res).doesNotContain(mConfig.keys.fullMessageSize);
        assertThat(res).doesNotContain(mConfig.keys.responseType);

    }

    @Test
    void generateFromUserToUserEnvelopes() throws Exception{
        String res = messagesBuilder.generateUser2UserFirstEnvelope("userID", "test", "single", "1024");
        assertThat(res).containsSubsequence(mConfig.keys.packageType + mConfig.delimiters.value + mConfig.packages.firstPart);
        assertThat(res).containsSubsequence(mConfig.keys.targetType + mConfig.delimiters.value + mConfig.targets.controlSide);
        assertThat(res).containsSubsequence(mConfig.keys.targetModule + mConfig.delimiters.value + "test");
        assertThat(res).containsSubsequence(mConfig.keys.targetID + mConfig.delimiters.value + "userID");
        assertThat(res).containsSubsequence(mConfig.keys.responseType + mConfig.delimiters.value + "single");
        assertThat(res).containsSubsequence(mConfig.keys.fullMessageSize + mConfig.delimiters.value + "1024");


        res = messagesBuilder.generateUser2UserSingleEnvelope("userID", "test", "single");
        assertThat(res).containsSubsequence(mConfig.keys.packageType + mConfig.delimiters.value + mConfig.packages.singleMessage);
        assertThat(res).containsSubsequence(mConfig.keys.targetType + mConfig.delimiters.value + mConfig.targets.controlSide);
        assertThat(res).containsSubsequence(mConfig.keys.targetModule + mConfig.delimiters.value + "test");
        assertThat(res).containsSubsequence(mConfig.keys.targetID + mConfig.delimiters.value + "userID");
        assertThat(res).containsSubsequence(mConfig.keys.responseType + mConfig.delimiters.value + "single");


        res = messagesBuilder.generateUser2UserEnvelope("userID", "test", "last");
        assertThat(res).containsSubsequence(mConfig.keys.packageType + mConfig.delimiters.value + mConfig.packages.lastPart);
        assertThat(res).containsSubsequence(mConfig.keys.targetType + mConfig.delimiters.value + mConfig.targets.controlSide);
        assertThat(res).containsSubsequence(mConfig.keys.targetModule + mConfig.delimiters.value + "test");
        assertThat(res).containsSubsequence(mConfig.keys.targetID + mConfig.delimiters.value + "userID");
    }

    @Test
    void generateFromBotToUserEnvelopes() throws Exception{

        String res = messagesBuilder.generateBot2UserFirstEnvelope("botId", "test", "requestId", "1024", "1");
        assertThat(res).containsSubsequence(mConfig.keys.packageType + mConfig.delimiters.value + mConfig.packages.firstPart);
        assertThat(res).containsSubsequence(mConfig.keys.targetType + mConfig.delimiters.value + mConfig.targets.botSide);
        assertThat(res).containsSubsequence(mConfig.keys.targetModule + mConfig.delimiters.value + "test");
        assertThat(res).containsSubsequence(mConfig.keys.targetID + mConfig.delimiters.value + "botId");
        assertThat(res).containsSubsequence(mConfig.keys.fullMessageSize + mConfig.delimiters.value + "1024");


        res = messagesBuilder.generateBot2UserSingleEnvelope("botId", "test", "requestId", "0");
        assertThat(res).containsSubsequence(mConfig.keys.packageType + mConfig.delimiters.value + mConfig.packages.singleMessage);
        assertThat(res).containsSubsequence(mConfig.keys.targetType + mConfig.delimiters.value + mConfig.targets.botSide);
        assertThat(res).containsSubsequence(mConfig.keys.targetModule + mConfig.delimiters.value + "test");
        assertThat(res).containsSubsequence(mConfig.keys.targetID + mConfig.delimiters.value + "botId");


        res = messagesBuilder.generateBot2UserEnvelope("botId", "test", "requestId", "last");
        assertThat(res).containsSubsequence(mConfig.keys.packageType + mConfig.delimiters.value + mConfig.packages.lastPart);
        assertThat(res).containsSubsequence(mConfig.keys.targetType + mConfig.delimiters.value + mConfig.targets.botSide);
        assertThat(res).containsSubsequence(mConfig.keys.targetModule + mConfig.delimiters.value + "test");
        assertThat(res).containsSubsequence(mConfig.keys.targetID + mConfig.delimiters.value + "botId");

    }

}
