//
// Created by al_sah on 05.03.21.
//

#ifndef BASIC_MODULE_WEBSOCKETRUNNER_H
#define BASIC_MODULE_WEBSOCKETRUNNER_H

#include "wsr_resources.h"

#ifdef headers_includes
class CommandsManager;
#endif

class WebsocketRunner : public Module {
private:

    wsr::ws_client client;
    wsr::thread thread; // client thread (messages listener)

    wsr::connection_metainfo metainfo = {};
    wsr::ws_runner_properties properties;

private:

    /// returns false in case if error caught, else -> true;
    [[nodiscard]] bool handleError(websocketpp::lib::error_code error_code);

    void on_close(const websocketpp::connection_hdl& hdl);
    void on_fail(const websocketpp::connection_hdl& hdl);
    void on_open(const websocketpp::connection_hdl& hdl);
    void on_message(const websocketpp::connection_hdl& hdl, const wsr::message_ptr& msg);

public:


    void executeTask(std::string payload, payload_type pt, std::function<void (payload_type, void *, bool)> callback) override;
    explicit WebsocketRunner(wsr::ws_runner_properties properties);
    ~WebsocketRunner();

#ifdef BOT_ENABLE
    bool setup_c onnection(const std::string &uri);
#else
    bool setup_connection(const std::string &uri, const std::string &pswd);
#endif


    bool send_message(const std::string &message);
    bool close_connection(wsr::close_status_code code = websocketpp::close::status::normal, const std::string& reason = "Normal closing");
    const wsr::connection_metainfo &getConnectionMetainfo() const;
    const wsr::ws_runner_properties &getProperties() const;

#ifdef headers_includes
private:
    std::weak_ptr<CommandsManager> commandsManager;
public:
    void setCommandsManager(const std::weak_ptr<CommandsManager> &commandsManager);
#else

private:
    std::function<void(std::string)> register_message;
    std::function<void(wsr::ws_runner_properties &)> wsRunnerPropertiesUpdater;
    std::function<void(wsr::connection_metainfo &)> connectionMetainfoUpdater;
public:
    void set_messages_register(std::function<void(std::string)> &function);
    void setWsRunnerPropertiesUpdater(const std::function<void(wsr::ws_runner_properties &)> &wsRunnerPropertiesUpdater);
    void setConnectionMetainfoUpdater(const std::function<void(wsr::connection_metainfo &)> &connectionMetainfoUpdater);
#endif

};


#endif //BASIC_MODULE_WEBSOCKETRUNNER_H
