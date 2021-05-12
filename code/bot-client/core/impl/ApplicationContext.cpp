//
// Created by al_sah on 09.03.21.
//

#include "../ApplicationContext.h"

ApplicationContext::ApplicationContext(){
    this->module_id = "ApplicationContext";
}

const WSRunnerProperties &ApplicationContext::getWsRunnerProperties() const {
    return wsRunnerProperties;
}

const CommandsManagerProperties &ApplicationContext::getCommandsManagerProperties() const {
    return commandsManagerProperties;
}

const ModulesManagerProperties &ApplicationContext::getModulesManagerProperties() const {
    return modulesManagerProperties;
}

void ApplicationContext::executeTask(std::string payload, payload_type pt, std::function<void(payload_type, void *, bool)> callback) {

}
