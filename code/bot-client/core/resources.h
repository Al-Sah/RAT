//
// Created by al_sah on 05.03.21.
//

#ifndef BASIC_MODULE_RESOURCES_H
#define BASIC_MODULE_RESOURCES_H

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include <string>
#include <memory>

typedef websocketpp::config::asio_client::message_type::ptr MessagePtr;

typedef websocketpp::client<websocketpp::config::asio_client> WSClient;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::thread> Thread;
typedef websocketpp::close::status::value CloseStatusCode;
typedef websocketpp::lib::error_code ErrorCode;

struct ConnectionMetainfo{

    /// Errors handling
    ErrorCode lastErrorCode;
    std::string lastErrorReason;

    websocketpp::connection_hdl hdl;

    std::string statusDetails;
    std::string status;

    std::string myID;
    std::string uri;

    CloseStatusCode  closeStatusCode;
};

struct WSRunnerProperties{


    std::string myID = "NEW";
    std::string uris[2] = {"ws://localhost:8080/bot", "ws://localhost:8888/bot"};

    uint32_t accessLoggingLevel = websocketpp::log::alevel::all;
    uint32_t errorsLoggingLevel = websocketpp::log::elevel::all;

    uint32_t reconnectTime = 5;

};




#endif //BASIC_MODULE_RESOURCES_H
