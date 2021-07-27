//
// Created by al_sah on 17.07.21.
//

#ifndef BOT_WEBSOCKET_RUNNER
#define BOT_WEBSOCKET_RUNNER

#include <WebSocket/AbstractWebsocketRunner.h>

class WebSocketRunner: public AbstractWebsocketRunner{

private:
    void onClose(const websocketpp::connection_hdl& hdl) override;
    void onOpen(const websocketpp::connection_hdl& hdl) override;
    void onFail(const websocketpp::connection_hdl &hdl) override;

public:
    WebSocketRunner(std::string moduleId, std::string version, wsr::WebSocketRunnerProperties &properties, wsr::ConnectionMetainfo &metainfo);

    bool setupConnection(const std::string &uri);
};

#endif //BOT_WEBSOCKET_RUNNER