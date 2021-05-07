//
// Created by al_sah on 09.03.21.
//

#include "../ModulesManager.h"
#include <iostream>
#include <utility>


#ifdef headers_includes
#include "../CommandsManager.h"
#endif

void ModulesManager::executeTask(std::string module, std::string task_id, std::shared_ptr<std::string> payload) {
    std::cout<< "Executing task\n Module ["<< module <<"] " << " Task_id ["<< task_id <<"] Payload ["<< payload.operator*() <<"] !!!";
    std::cout << std::flush;
    std::shared_ptr<std::string> new_payload = std::make_shared<std::string>(std::string ("New __Result__ useful info"));
    this->handleResult(task_id, new_payload);
}

void ModulesManager::handleResult(std::string &task_id, std::shared_ptr<std::string> payload, bool isLast) {
#ifdef headers_includes
    commandsManager.lock()->handleResponseMessage(task_id, std::move(payload), isLast);
#else
    this->result_handler(task_id, std::move(payload), isLast);
#endif
}



ModulesManager::ModulesManager(const ModulesManagerProperties &properties) : properties(properties) {}


#ifdef headers_includes
void ModulesManager::setCommandsManager(const std::weak_ptr<CommandsManager> &commandsManager) {
    ModulesManager::commandsManager = commandsManager;
}
#else
void ModulesManager::set_result_handler(std::function<void(std::string, std::shared_ptr<std::string>, bool)> &result_handler) {
    this->result_handler = result_handler;
}
#endif



