//
// Created by al_sah on 12.05.21.
//

#ifndef CORE_CM_RESOURCES_H
#define CORE_CM_RESOURCES_H

#include "configuration.h"
#include "models/ParsedTextMessage.h"
#include "models/TaskInfo.h"
#include "models/TaskResult.h"
#include <../modules/Module.h>

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <utility>
#include <unistd.h>
#include <list>

namespace cm{

    namespace command {

        struct delimiters{
            char section = '#';
            char value = ':';
        };

        struct keys {
            std::string response_type = "RT";
            std::string package_type = "PT";
            std::string target_id = "TID";
            std::string target_type = "TT";
            std::string request_id = "RID";
            std::string target_module = "TM";
            std::string is_last = "L";
            std::string full_payload_size = "FS";
        };

        struct packages {
            std::string first_part = "first";
            std::string continuation = "con";
            std::string last_part = "last";
            std::string single_message = "single";
        };



    }

    struct commands_manager_properties{
        bool use_keys = true;
        command::delimiters delimiters;
        command::packages packages;
        command::keys keys;
        Targets targets;
    };
}




#endif //CORE_CM_RESOURCES_H
