//
// Created by al_sah on 05.03.21.
//

#include "../WebsocketRunner.h"
#include <utility>

#ifdef headers_includes
#include "../CommandsManager.h"
void WebsocketRunner::setCommandsManager(const std::weak_ptr<CommandsManager> &commandsManager) {
    this->commandsManager = commandsManager;
}
#define REGISTER_MESSAGE(message)(commandsManager.lock()->add_new_message(message))
#else
void WebsocketRunner::set_messages_register(std::function<void(std::string)>& function) {
    this->register_message = function;
}
#define REGISTER_MESSAGE(message)(this->register_message(message))
#endif


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
    connection->set_max_message_size(this->properties.max_transferring_size);

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
    WSClient::connection_ptr con_ptr = client.get_con_from_hdl(hdl);
    this->metainfo.status = websocketpp::close::status::get_string(con_ptr->get_local_close_code());
    this->metainfo.statusDetails = con_ptr->get_local_close_reason();
    this->metainfo.closeStatusCode = con_ptr->get_local_close_code();

    std::cout << "\nConnection closed. close_code [" << this->metainfo.closeStatusCode
        << "] status ["<<this->metainfo.status << "] close_reason [" << this->metainfo.statusDetails << "]\n" << std::flush;

    sleep(5);
    this->setup_connection("ws://localhost:8080/bot");
}

void WebsocketRunner::on_open(const websocketpp::connection_hdl& hdl) {
    this->metainfo.status = "Opened";
    metainfo.max_transferring_size = this->properties.max_transferring_size;

    std::cout << "\nEstablished new connection. max_message_size ["<< client.get_con_from_hdl(hdl)->get_max_message_size()
    << "] Bot-id: [" << client.get_con_from_hdl(hdl)->get_response().get_header("bot-id") << "]\n" << std::flush;
}

void WebsocketRunner::on_message(const websocketpp::connection_hdl& hdl, const MessagePtr& msg) {
    std::cout<< "\nNew message: " << msg->get_payload() << std::endl;
    std::string message = msg->get_payload();
    REGISTER_MESSAGE(message);
}

void WebsocketRunner::on_fail(const websocketpp::connection_hdl &hdl) {
    this->metainfo.status = "Failed";
    this->metainfo.lastErrorCode = client.get_con_from_hdl(hdl)->get_ec();
    this->metainfo.lastErrorReason = client.get_con_from_hdl(hdl)->get_ec().message();
    std::cout << "failed to connect [" << metainfo.lastErrorCode.message() << "]" << std::endl;

    sleep(5);
    this->setup_connection("ws://localhost:8080/bot");
}

bool WebsocketRunner::send_message(const std::string &message) {
    ErrorCode ec;
    client.send(metainfo.hdl, message, websocketpp::frame::opcode::value::TEXT, ec);
    return !handleError(ec);
}

WebsocketRunner::WebsocketRunner(WSRunnerProperties properties) : properties(std::move(properties)) {
    client.clear_access_channels(properties.accessLoggingLevel);
    client.clear_error_channels(properties.errorsLoggingLevel);
    client.init_asio();
    client.start_perpetual();

    metainfo.myID = this->properties.myID;
    this->module_id = "WebsocketRunner";

    thread.reset(new websocketpp::lib::thread(&WSClient::run, &client));
}

void WebsocketRunner::executeTask(std::string payload, payload_type pt, std::function<void(payload_type, void *, bool)> callback) {
}