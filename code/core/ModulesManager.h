//
// Created by al_sah on 09.03.21.
//

#ifndef MODULES_MANAGER_H
#define MODULES_MANAGER_H

#include "mm_resources.h"
#include "models/ParsedTextMessage.h"

#ifdef CONTROL_ENABLE
#include "DataTransfer.h"
#endif

#ifdef headers_includes
class CommandsManager;
#endif


class ModulesManager : public Module{
private:

    mm::modules_manager_properties properties;

    std::function<void(payload_type, void*, void*)> default_modules_callback;

    std::map<std::string, Module*> modules;
    //std::list<Module*> modules;
    void loadExternalModules();

    Module* _findModule(std::string& id);

#ifdef CONTROL_ENABLE
    void * ui;
    DataTransfer* dataTransfer;
#endif

public:
    Module* findModule(std::string id);
    explicit ModulesManager(const mm::modules_manager_properties &properties, void * ui);
    void executeTask(std::string task, std::string payload, payload_type pt, std::function<void (payload_type, void *, bool)> callback) override;

    void handleTask(std::string& module, std::string& task_id, std::string& payload, payload_type pt);

    void handleTask(Module* module, payload_type pt, std::string payload);
    //void handleModuleAction(payload_type result, void* result_payload, std::string task_id, bool isLast);
    void handleModuleAction(payload_type result, void* result_payload, void* info);

    void registerModule(Module* module);
    void registerModule(getInstance_t);

    std::string getModules();
    virtual ~ModulesManager();


#ifdef headers_includes
private:
    std::weak_ptr<CommandsManager> commandsManager;
public:
    void setCommandsManager(const std::weak_ptr<CommandsManager> &commandsManager);
#else
private:
    std::function<void(TaskResult, ParsedTextMessage)> request_handler;
public:
    void set_result_handler(std::function<void(TaskResult,ParsedTextMessage)>& result_handler);
#endif
};


#endif //MODULES_MANAGER_H