//
// Created by al_sah on 17.07.21.
//

#ifndef BOT_MODULES_MANAGER
#define BOT_MODULES_MANAGER


#include <ModulesManager/AbstractModulesManager.h>
#include <TaskExecutor.h>

class ModulesManager: public AbstractModulesManager{

private:
    std::map<std::string, TaskExecutor*> modules;
public:
    void loadExternalModules() override;

    void handleTask(std::string &module, std::string & task_id, std::string& payload, PayloadType pt) override;
    void handleModuleAction(PayloadType result, void *result_payload, void* info) override;

    void registerModule(TaskExecutor* newModule);
    void registerModule(getModuleInstance function);

    std::string getModules();
    TaskExecutor * findModule(std::string& id);

    ModulesManager(std::string moduleId, std::string version, mm::ModulesManagerProperties &properties);
};


#endif //BOT_MODULES_MANAGER
