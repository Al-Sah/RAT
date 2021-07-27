//
// Created by al_sah on 17.07.21.
//

#include "../WebSocketRunner.h"


void WebSocketRunner::onClose(const websocketpp::connection_hdl& hdl) {
    AbstractWebsocketRunner::onClose(hdl);

    // TODO implement separate logger
    std::cout << "\nConnection closed. close_code [" << this->metainfo.closeStatusCode
              << "] status [" << this->metainfo.status << "] close_reason [" << this->metainfo.statusDetails << "]\n" << std::flush;
}

void WebSocketRunner::onOpen(const websocketpp::connection_hdl &hdl) {
    AbstractWebsocketRunner::onOpen(hdl);

    this->metainfo.myID = client.get_con_from_hdl(hdl)->get_response_header("user-id");
    std::cout << "\nEstablished new connection. max_message_size ["<< client.get_con_from_hdl(hdl)->get_max_message_size()
              << "] Bot-id: [" << client.get_con_from_hdl(hdl)->get_response().get_header("bot-id") << "]\n" << std::flush;
}

void WebSocketRunner::onFail(const websocketpp::connection_hdl &hdl) {
    AbstractWebsocketRunner::onFail(hdl);
    std::cout << "failed to connect [" << metainfo.lastErrorCode.message() << "]" << std::endl;
}

WebSocketRunner::WebSocketRunner(
        std::string moduleId, std::string version,
        wsr::WebSocketRunnerProperties &properties,
        wsr::ConnectionMetainfo &metainfo)
        : AbstractWebsocketRunner(moduleId, version, properties, metainfo) {
    qRegisterMetaType <wsr::ConnectionMetainfo>("wsr::ConnectionMetainfo");
}

bool WebSocketRunner::setupConnection(const std::string &uri, const std::string &pwd) {

    wsr::ErrorCode ec;
    wsr::ConnectionPtr connection = client.get_connection(uri, ec);
    if(errorCaught(ec)) return false;

    this->configureConnection(connection);
    connection->append_header("control-id", this->properties.myID);
    connection->append_header("authentication", pwd);
    client.connect(connection);

    this->metainfo.uri = uri;
    this->metainfo.hdl = connection->get_handle();
    return true;
}
