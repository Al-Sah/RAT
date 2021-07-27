//
// Created by al_sah on 12.05.21.
//

#ifndef CORE_WSR_RESOURCES_H
#define CORE_WSR_RESOURCES_H

#include "../CoreResources.h"

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

namespace wsr{

    typedef websocketpp::config::asio_client::message_type::ptr MessagePtr;
    typedef websocketpp::client<websocketpp::config::asio_client> Client;
    typedef websocketpp::client<websocketpp::config::asio_client>::connection_ptr ConnectionPtr;
    typedef websocketpp::lib::shared_ptr<websocketpp::lib::thread> Thread;
    typedef websocketpp::close::status::value CloseStatusCode;
    typedef websocketpp::lib::error_code ErrorCode;



    struct ConnectionMetainfo{


        /// Errors handling
        ErrorCode lastErrorCode;
        std::string lastErrorReason;

        websocketpp::connection_hdl hdl;
        Size maxTransferringSize = 524288;

        std::string statusDetails;
        std::string status;

        std::string myID;
        std::string uri;

        CloseStatusCode  closeStatusCode;
    };

    struct WebSocketRunnerProperties{

        Size maxTransferringSize = 131072;
        std::string myID = "NEW";
        std::string uris[2] = {"ws://localhost:8080/bot", "ws://localhost:8888/bot"};

        uint32_t accessLoggingLevel = websocketpp::log::alevel::all;
        uint32_t errorsLoggingLevel = websocketpp::log::elevel::all;

    };
}

#endif //CORE_WSR_RESOURCES_H