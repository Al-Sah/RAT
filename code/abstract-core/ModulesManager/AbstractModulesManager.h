//
// Created by al_sah on 09.03.21.
//

#ifndef MODULES_MANAGER_H
#define MODULES_MANAGER_H

#include "ModulesManagerResources.h"
#include "../models/ParsedTextMessage.h"
#include "../TaskManaging/TaskResultRegister.h"
#include "TaskExecutingDelegator.h"


class AbstractModulesManager : public Module, public TaskExecutingDelegator{

protected:

    TaskResultRegister *taskResultRegister{};
    mm::ModulesManagerProperties properties;

    std::function<void(PayloadType, void*, void*)> defaultCallback;


    virtual void loadExternalModules() = 0;

public:
    AbstractModulesManager(std::string &moduleId, std::string &version, mm::ModulesManagerProperties &properties);


    //void handleTask(TaskExecutor* module, PayloadType pt, std::string payload);
    virtual void handleModuleAction(PayloadType result, void* result_payload, void* info) = 0;


    void setTaskResultRegister(TaskResultRegister *taskResultRegister);
    virtual ~AbstractModulesManager() = default;
};

#endif //MODULES_MANAGER_H