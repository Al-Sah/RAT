//
// Created by al_sah on 09.03.21.
//

#include <iostream>
#include "../ModulesManager.h"

void ModulesManager::executeTask(std::string module, std::string task_id, std::shared_ptr<std::string> payload) {
    std::cout<< "Executing task\n Module ["<< module <<"] " << " Task_id ["<< task_id <<"] Payload ["<< payload.operator*() <<"] !!!";
    std::cout << std::flush;
}
