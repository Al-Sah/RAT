//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_MODULESMANAGER_H
#define BASIC_MODULE_MODULESMANAGER_H


#include "models/TaskResult.h"
#include <string>
#include <list>
#include <map>
#include <memory>
#include "resources.h"


#ifdef headers_includes
class CommandsManager;
#endif

class ModulesManager {

private:
    ModulesManagerProperties properties;
public:

    ModulesManager(const ModulesManagerProperties &properties);

    // TODO extern modules

    void handleTask(std::string& module, std::string& task_id, std::shared_ptr<std::string>& payload);
    void handleResult(std::string &task_id, std::string &payload, bool isLast = true);

    void findModule(std::string id); // TODO

    void executeTask(/*Module,*/ std::string payload, std::string taskId); // TODO

#ifdef headers_includes
private:
    std::weak_ptr<CommandsManager> commandsManager;
public:
    void setCommandsManager(const std::weak_ptr<CommandsManager> &commandsManager);
#else
private:
    std::function<void(TaskResult)> result_handler;
public:
    void set_result_handler(std::function<void(TaskResult)>& result_handler);
#endif



};


#endif //BASIC_MODULE_MODULESMANAGER_H
