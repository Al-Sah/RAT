//
// Created by al_sah on 12.05.21.
//

#ifndef CORE_CM_RESOURCES_H
#define CORE_CM_RESOURCES_H

#include "../CoreResources.h"

#include <fstream>
#include <mutex>
#include <thread>
#include <list>
#include <queue>
#include <iostream>
#include <utility>


namespace cm{

    namespace command {
        struct Delimiters{
            char section = '#';
            char value = ':';
        };

        struct Keys {
            std::string responseType = "RT";
            std::string packageType = "PT";
            std::string targetId = "TID";
            std::string targetType = "TT";
            std::string requestId = "RID";
            std::string targetModule = "TM";
            std::string isLast = "L";
            std::string fullPayloadSize = "FS";
        };

        struct Packages {
            std::string firstPart = "first";
            std::string continuation = "con";
            std::string lastPart = "last";
            std::string singleMessage = "single";
        };
    }
    struct CommandsManagerProperties{
        bool useKeys = true;
        command::Delimiters delimiters;
        command::Packages packages;
        command::Keys keys;
        TargetsIds targets;
    };
}

#endif //CORE_CM_RESOURCES_H