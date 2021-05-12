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
#include "modules/Module.h"


#ifdef headers_includes
class CommandsManager;
#endif

class ModulesManager : public Module{
private:

    ModulesManagerProperties properties;

    //std::map<std::string, Module*> modules;
    std::list<Module*> modules;
    void loadExternalModules();

    Module* findModule(std::string& id);

public:
    explicit ModulesManager(const ModulesManagerProperties &properties);
    void executeTask(std::string payload, payload_type pt, std::function<void (payload_type, void *, bool)> callback) override;

    void handleTask(std::string& module, std::string& task_id, std::shared_ptr<std::string>& payload);
    void handleResult(payload_type result, void* result_payload, std::string task_id, bool isLast);

    void registerModule(Module* module);




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
