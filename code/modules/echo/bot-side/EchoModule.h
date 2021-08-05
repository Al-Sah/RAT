//
// Created by al_sah on 04.04.21.
//

#ifndef ECHO_MODULE_BOT_SIDE
#define ECHO_MODULE_BOT_SIDE

#include "TaskExecutor.h"

class EchoModule : public TaskExecutor {

public:
    EchoModule(std::function<void(PayloadType, void*, void*)> &callback, void* data);
    void executeTask(std::string task, std::string payload, PayloadType pt, std::function<void(PayloadType, void*, bool)> callback) override;
};


#endif //ECHO_MODULE_BOT_SIDE