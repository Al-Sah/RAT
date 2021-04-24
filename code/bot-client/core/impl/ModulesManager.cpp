//
// Created by al_sah on 09.03.21.
//

#include <iostream>
#include <utility>
#include "../ModulesManager.h"
#include "../CommandsManager.h"

void ModulesManager::executeTask(std::string module, std::string task_id, std::shared_ptr<std::string> payload) {
    std::cout<< "Executing task\n Module ["<< module <<"] " << " Task_id ["<< task_id <<"] Payload ["<< payload.operator*() <<"] !!!";
    std::cout << std::flush;
    std::shared_ptr<std::string> new_payload = std::make_shared<std::string>(std::string ("New __Result__ useful info"));
    this->handleResult(task_id, new_payload);
}

void ModulesManager::handleResult(std::string &task_id, std::shared_ptr<std::string> payload, bool isLast) {
    commandsManager.lock()->handleResponseMessage(task_id, std::move(payload), isLast);
}

void ModulesManager::setCommandsManager(const std::weak_ptr<CommandsManager> &commandsManager) {
    ModulesManager::commandsManager = commandsManager;
}

ModulesManager::ModulesManager(const ModulesManagerProperties &properties) : properties(properties) {}
