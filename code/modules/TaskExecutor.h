//
// Created by al_sah on 16.07.21.
//

#ifndef TASK_EXECUTOR_MODULE
#define TASK_EXECUTOR_MODULE

#include "Module.h"

class TaskExecutor : public Module {
public:
    // TODO send task id ?
    virtual void executeTask(std::string task, std::string payload, PayloadType pt, std::function<void(PayloadType, void*, bool)> callback) = 0;


};

using getExecutorInstance = TaskExecutor* (*)(std::function<void(PayloadType, void*, void*)> &callback, void* data);
#endif //TASK_EXECUTOR_MODULE