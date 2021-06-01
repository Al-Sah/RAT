//
// Created by al_sah on 12.05.21.
//

#ifndef CORE_MODULE_RESOURCES_H
#define CORE_MODULE_RESOURCES_H

#include "../core/models/TaskResult.h"
#include "../core/models/payload_type.h"
#include "../core/models/Targets.h"

#include <functional>

#include <sstream>
#include <random>

struct botResult{
    std::string task_id;
    bool isLast;
};

struct controlRequest{
    std::string task_id;
    std::string target_id;
    targets_enum target_type;
    std::string target_module;
    std::string required_response;
};

std::string generate_uuid_v4();

#endif //CORE_MODULE_RESOURCES_H
