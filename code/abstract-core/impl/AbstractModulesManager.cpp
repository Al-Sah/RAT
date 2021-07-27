//
// Created by al_sah on 09.03.21.
//


#include "../ModulesManager/AbstractModulesManager.h"

/*void AbstractModulesManager::handleTask(TaskExecutor *module, PayloadType pt, std::string payload) {
    std::thread thread(&TaskExecutor::executeTask, module, "", payload, pt, nullptr);
    thread.detach();
}*/

/*
std::string AbstractModulesManager::getModules() {
    std::string res = "botModules ";
    for(const auto& module: modules){
        res = res.append(module.first);
        res = res.append(" ");
    }
    return res;
}
*/


AbstractModulesManager::AbstractModulesManager(
        std::string &moduleId, std::string &version,
        mm::ModulesManagerProperties &properties)
        : Module(moduleId, version), properties(properties) {

    this->defaultCallback = [this](PayloadType pt, void* payload, void* data){
        this->handleModuleAction(pt, payload, data);
    };
}

void AbstractModulesManager::setTaskResultRegister(TaskResultRegister *taskResultRegister) {
    AbstractModulesManager::taskResultRegister = taskResultRegister;
}

