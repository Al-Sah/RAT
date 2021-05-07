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


#ifdef headers_includes
class CommandsManager;
#endif

class ModulesManager {

private:
    ModulesManagerProperties properties;
public:

    ModulesManager(const ModulesManagerProperties &properties);


    // TODO extern modules
// temp
    void executeTask(std::string module, std::string task_id, std::shared_ptr<std::string> payload);
    void handleResult(std::string &task_id, std::shared_ptr<std::string> payload, bool isLast = true);

#ifdef headers_includes
private:
    std::weak_ptr<CommandsManager> commandsManager;
public:
    void setCommandsManager(const std::weak_ptr<CommandsManager> &commandsManager);
#else
private:
    std::function<void(std::string,std::shared_ptr<std::string>,bool)> result_handler;
public:
    void set_result_handler(std::function<void(std::string,std::shared_ptr<std::string>,bool)>& result_handler);
#endif



};


#endif //BASIC_MODULE_MODULESMANAGER_H
