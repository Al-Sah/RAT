//
// Created by al_sah on 05.03.21.
//

#ifndef WEBSOCKET_RUNNER_MODULE
#define WEBSOCKET_RUNNER_MODULE

#include "../TaskManaging/InboxTasksRegister.h"
#include "WebSocketMessageSender.h"
#include "WebSocketResources.h"

class AbstractWebsocketRunner : public Module, public WebSocketMessageSender {

protected:
    wsr::Client client; // websocketpp library class
    wsr::Thread thread; // client Thread (messages listener)

    wsr::ConnectionMetainfo& metainfo;
    wsr::WebSocketRunnerProperties& properties;

    InboxTasksRegister*   taskRegister{}; // TaskManager interface


    /// returns true in case if error caught, else -> false;
    [[nodiscard]] bool errorCaught(websocketpp::lib::error_code error_code);
    void configureConnection(wsr::ConnectionPtr& connection);

    virtual void onClose(const websocketpp::connection_hdl& hdl);
    virtual void onFail(const websocketpp::connection_hdl& hdl);
    virtual void onOpen(const websocketpp::connection_hdl& hdl);
    virtual void onMessage(const websocketpp::connection_hdl& hdl, const wsr::MessagePtr& msg);

public:

    AbstractWebsocketRunner(std::string &moduleId, std::string &version,
                            wsr::WebSocketRunnerProperties &properties, wsr::ConnectionMetainfo &metainfo);
    virtual ~AbstractWebsocketRunner();
    void setTaskRegister(InboxTasksRegister *taskRegister);


    bool send(const std::string &message, PayloadType pt) override;
    Size getMaxTransferringSize() override;
    bool closeConnection(wsr::CloseStatusCode code, const std::string& reason);

};

#endif //WEBSOCKET_RUNNER_MODULE