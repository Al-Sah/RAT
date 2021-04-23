//
// Created by al_sah on 05.03.21.
//

#include "../WebsocketRunner.h"

WebsocketRunner::WebsocketRunner(const WSRunnerProperties& properties, std::weak_ptr<CommandsManager> commandsManager) {
    client.clear_access_channels(properties.accessLoggingLevel);
    client.clear_error_channels(properties.errorsLoggingLevel);

    client.init_asio();
    client.start_perpetual();

    this->properties = properties;
    metainfo = {};
    metainfo.myID = properties.myID;
    this->commandsManager = commandsManager;

    thread.reset(new websocketpp::lib::thread(&WSClient::run, &client));
}

WebsocketRunner::~WebsocketRunner() {

    client.stop_perpetual();
    thread->join();
}

bool WebsocketRunner::setup_connection(const std::string &uri) {

    ErrorCode ec;
    WSClient::connection_ptr connection = client.get_connection(uri, ec);
    this->metainfo.uri = uri;
    if(!handleError(ec)) return false;

     /* Each handle can be set using bind: websocketpp::lib::bind(&WebsocketRunner::method, this, placeholders...)
     Previously was used: on_something(std::forward<decltype(connection_handle)>(connection_handle));
     Lambda:   std::function<void(std::weak_ptr<void>)>  where std::weak_ptr<void> equals connection_hdl */
    connection->set_max_message_size(524288);

    connection->set_close_handler([this](auto && connection_handle){
        this->on_close(connection_handle);
    });

    connection->set_open_handler([this](auto && connection_handle) {
        this->on_open(connection_handle);
    });

    connection->set_fail_handler([this](auto && connection_handle) {
        this->on_fail(connection_handle);
    });

    //MessagePtr equals websocketpp::config::asio_client::message_type::ptr
    connection->set_message_handler([this](auto && connection_handle, auto && MessagePtr){
        this->on_message(connection_handle, MessagePtr);
    });

    connection->append_header("bot-id", this->properties.myID);


    client.connect(connection);
    this->metainfo.hdl = connection->get_handle();
    return true;
}

bool WebsocketRunner::close_connection(CloseStatusCode code, const std::string& reason) {

    ErrorCode ec;
    client.close(this->metainfo.hdl, code, reason, ec);
    return !handleError(ec);
}


bool WebsocketRunner::handleError(websocketpp::lib::error_code error_code) {
    if (error_code) {
        metainfo.lastErrorCode = error_code;
        metainfo.lastErrorReason = error_code.message();
        return false;
    }else{
        return true;
    }
}

const ConnectionMetainfo &WebsocketRunner::getConnectionMetainfo() const {
    return metainfo;
}

void WebsocketRunner::on_close(const websocketpp::connection_hdl& hdl) {
    this->metainfo.status = "Closed";
    std::stringstream s;
    WSClient::connection_ptr connection_ptr = client.get_con_from_hdl(hdl);


    std::cout
            << "\nget_remote_close_code " << connection_ptr->get_remote_close_code() << ")\n"
            << "status ("<< websocketpp::close::status::get_string(connection_ptr->get_remote_close_code())<< ")\n"
            <<" get_remote_close_reason: " << connection_ptr->get_remote_close_reason();

    std::cout
            << "\nget_local_close_code " << connection_ptr->get_local_close_code() << ")\n"
            << "status ("<< websocketpp::close::status::get_string(connection_ptr->get_local_close_code())<< ")\n"
            <<" get_local_close_reason: " << connection_ptr->get_local_close_reason();

}

void WebsocketRunner::on_open(const websocketpp::connection_hdl& hdl) {
    this->metainfo.status = "Opened";

    WSClient::connection_ptr connection_ptr = client.get_con_from_hdl(hdl);
    std::cout << "max_message_size " << connection_ptr->get_max_message_size()<<std::endl;
    std::cout << "Opened connection: \nRequest:\n raw:\n" << connection_ptr->get_request().raw();
    std::cout << "Opened connection: \nResponse:\n raw:\n" << connection_ptr->get_response().raw() << "\nget_response_msg  " << connection_ptr->get_response_msg();
}


void WebsocketRunner::on_message(const websocketpp::connection_hdl& hdl, const MessagePtr& msg) {
    std::cout
            << "\nReceived message. HDL: " << hdl.lock().get() << " \nMessage payload: " << msg->get_payload()
            << "\nMessage header: " << msg->get_header()
            << "\nMessage opcode: " << msg->get_opcode() << std::endl;

    WSClient::connection_ptr connection_ptr = client.get_con_from_hdl(hdl);

    msg->get_opcode();

    commandsManager.lock()->handleMessage(msg->get_payload());
    //commandsManager->handleMessage(msg->get_payload());
}

void WebsocketRunner::on_fail(const websocketpp::connection_hdl &hdl) {
    this->metainfo.status = "Failed";

    WSClient::connection_ptr connection_ptr = client.get_con_from_hdl(hdl);

    this->metainfo.lastErrorCode = connection_ptr->get_ec();
/*    this->metainfo.statusDetails = websocketpp::close::status::get_string(connection_ptr.);*/
    this->metainfo.lastErrorReason = connection_ptr->get_ec().message();
    //std::this_thread::sleep_for();
}

bool WebsocketRunner::send_message(const std::string &message) {

    ErrorCode ec;
    client.send(metainfo.hdl,message, websocketpp::frame::opcode::value::TEXT, ec);
    //client.send(metainfo.hdl,message, websocketpp::frame::opcode::value::CLOSE, ec);
    return !handleError(ec);
}
