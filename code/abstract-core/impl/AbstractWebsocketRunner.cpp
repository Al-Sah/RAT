//
// Created by al_sah on 05.03.21.
//

#include "../WebSocket/AbstractWebsocketRunner.h"

AbstractWebsocketRunner::AbstractWebsocketRunner(
        std::string& moduleId, std::string& version,
        wsr::WebSocketRunnerProperties& properties,
        wsr::ConnectionMetainfo& metainfo)
        : Module(moduleId, version), metainfo(metainfo), properties(properties) {

    client.clear_access_channels(properties.accessLoggingLevel);
    client.clear_error_channels(properties.errorsLoggingLevel);
    client.init_asio();
    client.start_perpetual();

    this->metainfo.myID = this->properties.myID;

    thread.reset(new websocketpp::lib::thread(&wsr::Client::run, &client));
}

AbstractWebsocketRunner::~AbstractWebsocketRunner() {
    closeConnection(websocketpp::close::status::going_away, "called WebsocketRunner destructor");
    client.stop_perpetual();
    thread->join();
}

bool AbstractWebsocketRunner::send(const std::string &message, PayloadType pt) {
    wsr::ErrorCode ec;
    if(pt == PayloadType::text){
        client.send(metainfo.hdl, message, websocketpp::frame::opcode::value::TEXT, ec);
    } else{
        client.send(metainfo.hdl, message, websocketpp::frame::opcode::value::BINARY, ec);
    }
    return errorCaught(ec);
}

bool AbstractWebsocketRunner::closeConnection(wsr::CloseStatusCode code, const std::string& reason) {
    wsr::ErrorCode ec;
    client.close(this->metainfo.hdl, code, reason, ec);
    return errorCaught(ec);
}

bool AbstractWebsocketRunner::errorCaught(websocketpp::lib::error_code error_code) {
    if (error_code) {
        metainfo.lastErrorCode = error_code;
        metainfo.lastErrorReason = error_code.message();
        return true;
    }else {
        return false;
    }
}


void AbstractWebsocketRunner::onClose(const websocketpp::connection_hdl& hdl) {
    wsr::ConnectionPtr connectionPtr = client.get_con_from_hdl(hdl);
    this->metainfo.status = websocketpp::close::status::get_string(connectionPtr->get_local_close_code());
    this->metainfo.statusDetails = connectionPtr->get_local_close_reason();
    this->metainfo.closeStatusCode = connectionPtr->get_local_close_code();
}

void AbstractWebsocketRunner::onOpen(const websocketpp::connection_hdl& hdl) {
    this->metainfo.status = "Connected";
    this->metainfo.statusDetails = "Connection established successfully";
    this->metainfo.maxTransferringSize = this->properties.maxTransferringSize;
}

void AbstractWebsocketRunner::onMessage(const websocketpp::connection_hdl& hdl, const wsr::MessagePtr& msg) {
    std::string message = msg->get_payload();
    if(msg->get_opcode() == websocketpp::frame::opcode::value::BINARY){
        taskRegister->registerInboxTask(message, PayloadType::binaryData);
    } else{
        taskRegister->registerInboxTask(message, PayloadType::text);
    }
}

void AbstractWebsocketRunner::onFail(const websocketpp::connection_hdl &hdl) {
    this->metainfo.status = "Failed";
    this->metainfo.lastErrorCode = client.get_con_from_hdl(hdl)->get_ec();
    this->metainfo.lastErrorReason = client.get_con_from_hdl(hdl)->get_ec().message();
    this->metainfo.statusDetails = metainfo.lastErrorCode.message();
}


size_t AbstractWebsocketRunner::getMaxTransferringSize() {
    return this->properties.maxTransferringSize;
}

void AbstractWebsocketRunner::configureConnection(wsr::ConnectionPtr& connection) {
    /* Each handle can be set using bind: websocketpp::lib::bind(&AbstractWebsocketRunner::method, this, placeholders...)
    Previously was used: on_something(std::forward<decltype(connection_handle)>(connection_handle));
    Lambda:   std::function<void(std::weak_ptr<void>)>  where std::weak_ptr<void> equals connection_hdl */
    connection->set_max_message_size(this->properties.maxTransferringSize);

    connection->set_close_handler([this](auto && connectionHandle){
        this->onClose(connectionHandle);
    });

    connection->set_open_handler([this](auto && connectionHandle) {
        this->onOpen(connectionHandle);
    });

    connection->set_fail_handler([this](auto && connectionHandle) {
        this->onFail(connectionHandle);
    });

    //MessagePtr equals websocketpp::config::asio_client::message_type::ptr
    connection->set_message_handler([this](auto && connectionHandle, auto && MessagePtr){
        this->onMessage(connectionHandle, MessagePtr);
    });
}


void AbstractWebsocketRunner::setTaskRegister(InboxTasksRegister *inboxTaskRegister) {
    AbstractWebsocketRunner::taskRegister = inboxTaskRegister;
}