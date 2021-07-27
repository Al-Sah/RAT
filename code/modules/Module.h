//
// Created by al_sah on 01.04.21.
//

#ifndef RAT_BASIC_MODULE
#define RAT_BASIC_MODULE

#include <iostream>
#include "GlobalResources.h"

class Module {
protected:
    std::string moduleId;
    std::string version;
public:

    std::string getModuleId() const{
        return this->moduleId;
    }
    std::string getVersion() const {
        return this->version;
    }

    Module(std::string& moduleId, std::string& version){
        this->moduleId = moduleId;
        this->version = version;
    }
    Module() = default;
};

using getModuleInstance = Module* (*)(std::function<void(PayloadType, void*, void*)> &callback, void* data);

#endif //RAT_BASIC_MODULE
