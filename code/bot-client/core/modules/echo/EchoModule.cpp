//
// Created by al_sah on 04.04.21.
//

#include "EchoModule.h"


EchoModule::EchoModule() {
    this->id = "echo";
}

std::string EchoModule::getId() const {
    return this->id;
}


void EchoModule::executeTask(std::string payload, payload_type pt, std::function<void(payload_type, void*, bool)> callback) {
    payload.append("(Echo)");
    callback(payload_type::text, &payload, true);
}

/*TaskResult EchoModule::executeTask(std::string payload) {
    payload.append("(Echo)");
    return TaskResult(payload, false, true);
}*/
