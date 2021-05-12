//
// Created by al_sah on 01.04.21.
//

#ifndef BASIC_MODULE_BOT_MODULE_H
#define BASIC_MODULE_BOT_MODULE_H

#include "../models/TaskResult.h"
#include "../resources.h"
#include <functional>

class Module {

protected:
    std::string module_id;

public:

    virtual std::string getId() const{
        return this->module_id;
    }
    //virtual TaskResult executeTask(std::string payload);
    virtual void executeTask(std::string payload, payload_type pt, std::function<void(payload_type, void*, bool)> callback) = 0;


};

//typedef Module* (*getInstance_t)();
using getInstance_t = Module* (*)();

#endif //BASIC_MODULE_BOT_MODULE_H
