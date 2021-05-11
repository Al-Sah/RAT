//
// Created by al_sah on 09.03.21.
//

#include "../ModulesManager.h"
#include <iostream>


#ifdef headers_includes
#include "../CommandsManager.h"
#endif

void ModulesManager::handleTask(std::string &module, std::string & task_id, std::shared_ptr<std::string> & payload_p) {
    std::cout<< "Executing task\n Module ["<< module <<"] " << " Task_id ["<< task_id <<"] Payload ["<< payload_p.operator*() <<"] !!!" << std::flush;
    std::string payload = *payload_p;
    std::thread thread(&ModulesManager::executeTask, this, payload, task_id);
    thread.detach();
}


void ModulesManager::executeTask(std::string payload, std::string task) {
    std::string new_payload = "New __Result__ useful info";
    // TODO Find module and call "run" function
    this->handleResult(task, new_payload);
}


void ModulesManager::handleResult(std::string &task_id, std::string &payload, bool isLast) {
    TaskResult message(task_id, payload, isLast);
#ifdef headers_includes
    commandsManager.lock()->handleResponseMessage(message);
#else
    this->result_handler(message);
#endif
}



ModulesManager::ModulesManager(const ModulesManagerProperties &properties) : properties(properties) {}


#ifdef headers_includes
void ModulesManager::setCommandsManager(const std::weak_ptr<CommandsManager> &commandsManager) {
    ModulesManager::commandsManager = commandsManager;
}
#else
void ModulesManager::set_result_handler(std::function<void(TaskResult)> &result_handler) {
    this->result_handler = result_handler;
}
#endif



