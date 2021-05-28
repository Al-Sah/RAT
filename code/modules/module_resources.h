//
// Created by al_sah on 12.05.21.
//

#ifndef CORE_MODULE_RESOURCES_H
#define CORE_MODULE_RESOURCES_H

#include "../core/models/TaskResult.h"
#include "../core/models/payload_type.h"

#include <functional>

struct botResult{
    std::string task_id;
    bool isLast;
};

struct controlRequest{
    std::string target_id;
    std::string target_type;
    std::string target_module;
    std::string required_response;
};


#endif //CORE_MODULE_RESOURCES_H
