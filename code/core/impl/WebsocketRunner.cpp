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

#ifdef BOT_ENABLE
bool WebsocketRunner::setup_connection(const std::string &uri) {
#else
    bool WebsocketRunner::setup_connection(const std::string &uri, const std::string &pswd) {
#endif

    wsr::error_code ec;
    wsr::ws_client::connection_ptr connection = client.get_connection(uri, ec);
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

    //message_ptr equals websocketpp::config::asio_client::message_type::ptr
    connection->set_message_handler([this](auto && connection_handle, auto && MessagePtr){
        this->on_message(connection_handle, MessagePtr);
    });

#ifdef BOT_ENABLE
    connection->append_header("bot-id", this->properties.myID);
#else
    connection->append_header("control-id", this->properties.myID);
    connection->append_header("authentication", pswd);
#endif

    client.connect(connection);
    this->metainfo.hdl = connection->get_handle();

#ifdef CONTROL_ENABLE
    connectionMetainfoUpdater(this->metainfo);
#endif

    return true;
}

bool WebsocketRunner::close_connection(wsr::close_status_code code, const std::string& reason) {
    wsr::error_code ec;
    client.close(this->metainfo.hdl, code, reason, ec);
    return !handleError(ec);
}


bool WebsocketRunner::handleError(websocketpp::lib::error_code error_code) {
    if (error_code) {
        metainfo.last_error_code = error_code;
        metainfo.last_error_reason = error_code.message();
        return false;
    }else{
        return true;
    }
}

const wsr::connection_metainfo &WebsocketRunner::getConnectionMetainfo() const {
    return metainfo;
}

void WebsocketRunner::on_close(const websocketpp::connection_hdl& hdl) {
    wsr::ws_client::connection_ptr con_ptr = client.get_con_from_hdl(hdl);
    this->metainfo.status = websocketpp::close::status::get_string(con_ptr->get_local_close_code());
    this->metainfo.status_details = con_ptr->get_local_close_reason();
    this->metainfo.closeStatusCode = con_ptr->get_local_close_code();

    std::cout << "\nConnection closed. close_code [" << this->metainfo.closeStatusCode
              << "] status [" << this->metainfo.status << "] close_reason [" << this->metainfo.status_details << "]\n" << std::flush;

#ifdef BOT_ENABLE
    if(this->metainfo.closeStatusCode != websocketpp::close::status::normal){
        sleep(5);
        this->setup_connection("ws://localhost:8080/bot");
    }

#endif
}

void WebsocketRunner::on_open(const websocketpp::connection_hdl& hdl) {
    this->metainfo.status = "Opened";
    metainfo.max_transferring_size = this->properties.max_transferring_size;

    std::cout << "\nEstablished new connection. max_message_size ["<< client.get_con_from_hdl(hdl)->get_max_message_size()
    << "] Bot-id: [" << client.get_con_from_hdl(hdl)->get_response().get_header("bot-id") << "]\n" << std::flush;
}

void WebsocketRunner::on_message(const websocketpp::connection_hdl& hdl, const wsr::message_ptr& msg) {
    std::cout<< "\nNew message: " << msg->get_payload() << std::endl;
    std::string message = msg->get_payload();
    REGISTER_MESSAGE(message);
}

void WebsocketRunner::on_fail(const websocketpp::connection_hdl &hdl) {
    this->metainfo.status = "Failed";
    this->metainfo.last_error_code = client.get_con_from_hdl(hdl)->get_ec();
    this->metainfo.last_error_reason = client.get_con_from_hdl(hdl)->get_ec().message();
    std::cout << "failed to connect [" << metainfo.last_error_code.message() << "]" << std::endl;

#ifdef BOT_ENABLE
    sleep(5);
    this->setup_connection("ws://localhost:8080/bot");
#endif
}

bool WebsocketRunner::send_message(const std::string &message) {
    wsr::error_code ec;
    client.send(metainfo.hdl, message, websocketpp::frame::opcode::value::TEXT, ec);
    return !handleError(ec);
}

WebsocketRunner::WebsocketRunner(wsr::ws_runner_properties properties) : properties(std::move(properties)) {
    client.clear_access_channels(properties.access_logging_level);
    client.clear_error_channels(properties.errors_logging_level);
    client.init_asio();
    client.start_perpetual();

    metainfo.myID = this->properties.myID;
    this->module_id = "WebsocketRunner";

    thread.reset(new websocketpp::lib::thread(&wsr::ws_client::run, &client));
}

void WebsocketRunner::executeTask(std::string payload, payload_type pt, std::function<void(payload_type, void *, bool)> callback) {
}

void WebsocketRunner::setWsRunnerPropertiesUpdater(
        const std::function<void(wsr::ws_runner_properties &)> &wsRunnerPropertiesUpdater) {
    WebsocketRunner::wsRunnerPropertiesUpdater = wsRunnerPropertiesUpdater;
}

void WebsocketRunner::setConnectionMetainfoUpdater(
        const std::function<void(wsr::connection_metainfo &)> &connectionMetainfoUpdater) {
    WebsocketRunner::connectionMetainfoUpdater = connectionMetainfoUpdater;
}
