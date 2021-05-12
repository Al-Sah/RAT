//
// Created by al_sah on 12.05.21.
//

#ifndef CORE_WSR_RESOURCES_H
#define CORE_WSR_RESOURCES_H

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include "modules/Module.h"
#include "configuration.h"

namespace wsr{

    typedef websocketpp::config::asio_client::message_type::ptr message_ptr;
    typedef websocketpp::client<websocketpp::config::asio_client> ws_client;
    typedef websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread;
    typedef websocketpp::close::status::value close_status_code;
    typedef websocketpp::lib::error_code error_code;

    struct connection_metainfo{

        /// Errors handling
        error_code last_error_code;
        std::string last_error_reason;

        websocketpp::connection_hdl hdl;

        size_t max_transferring_size = 524288;

        std::string status_details;
        std::string status;

        std::string myID;
        std::string uri;

        close_status_code  closeStatusCode;
    };

    struct ws_runner_properties{

        size_t max_transferring_size = 524288;
        std::string myID = "NEW";
        std::string uris[2] = {"ws://localhost:8080/bot", "ws://localhost:8888/bot"};

        uint32_t access_logging_level = websocketpp::log::alevel::all;
        uint32_t errors_logging_level = websocketpp::log::elevel::all;

    };
}

#endif //CORE_WSR_RESOURCES_H