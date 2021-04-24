//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_MODULESMANAGER_H
#define BASIC_MODULE_MODULESMANAGER_H


#include <string>
#include <list>
#include <map>
#include <memory>
#include "resources.h"

class CommandsManager;
class ModulesManager {

private:
    ModulesManagerProperties properties;
    std::weak_ptr<CommandsManager> commandsManager;
public:

    ModulesManager(const ModulesManagerProperties &properties);

    void setCommandsManager(const std::weak_ptr<CommandsManager> &commandsManager);
    // TODO extern modules
// temp
    void executeTask(std::string module, std::string task_id, std::shared_ptr<std::string> payload);
    void handleResult(std::string &task_id, std::shared_ptr<std::string> payload, bool isLast = true);


};


#endif //BASIC_MODULE_MODULESMANAGER_H
