//
// Created by al_sah on 05.03.21.
//

#ifndef BASIC_MODULE_WEBSOCKETRUNNER_H
#define BASIC_MODULE_WEBSOCKETRUNNER_H


#include "resources.h"
//#include "CommandsManager.h"

class CommandsManager;

class WebsocketRunner {

private:

    WSClient client;
    Thread thread;

    ConnectionMetainfo metainfo;
    WSRunnerProperties properties;

    std::weak_ptr<CommandsManager> commandsManager;
public:
    void setCommandsManager(const std::weak_ptr<CommandsManager> &commandsManager);

private:

    /// returns false in case if error caught, else -> true;
    [[nodiscard]] bool handleError(websocketpp::lib::error_code error_code);

    void on_close(const websocketpp::connection_hdl& hdl);
    void on_fail(const websocketpp::connection_hdl& hdl);
    void on_open(const websocketpp::connection_hdl& hdl);
    void on_message(const websocketpp::connection_hdl& hdl, const MessagePtr& msg);

public:
    WebsocketRunner(WSRunnerProperties properties);

    ~WebsocketRunner();

    bool setup_connection(const std::string &uri);
    bool send_message(const std::string &message);
    bool close_connection(CloseStatusCode code = websocketpp::close::status::normal, const std::string& reason = "Normal closing");

    const ConnectionMetainfo &getConnectionMetainfo() const;
};


#endif //BASIC_MODULE_WEBSOCKETRUNNER_H
