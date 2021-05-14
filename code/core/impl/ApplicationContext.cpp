//
// Created by al_sah on 09.03.21.
//

#include "../ApplicationContext.h"

ApplicationContext::ApplicationContext(){
    this->module_id = "ApplicationContext";
}

const wsr::ws_runner_properties &ApplicationContext::getWsRunnerProperties() const {
    return ws_runner_properties;
}

const cm::commands_manager_properties  &ApplicationContext::getCommandsManagerProperties() const {
    return commands_manager_properties;
}

const mm::modules_manager_properties &ApplicationContext::getModulesManagerProperties() const {
    return modules_manager_properties;
}

void ApplicationContext::executeTask(std::string payload, payload_type pt, std::function<void(payload_type, void *, bool)> callback) {

}
