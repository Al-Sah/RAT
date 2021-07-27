//
// Created by al_sah on 17.07.21.
//

#ifndef BOT_MODULES_MANAGER
#define BOT_MODULES_MANAGER


#include <ModulesManager/AbstractModulesManager.h>
#include <UIModule.h>
#include "DataTransfer.h"

class ModulesManager: public AbstractModulesManager{

private:
    std::map<std::string, UIModule*> modules;
    QWidget *ui;

public:
    void loadExternalModules() override;

    void handleTask(std::string &module, std::string & task_id, std::string& payload, PayloadType pt) override;
    void handleModuleAction(PayloadType result, void *result_payload, void* info) override;

    void registerModule(UIModule* newModule);
    void registerModule(getModuleInstance function);

    UIModule * findModule(std::string id);

    ModulesManager(std::string moduleId, std::string version,
                   mm::ModulesManagerProperties &properties, QWidget *ui);

    DataTransfer* dataTransfer;
};


#endif //BOT_MODULES_MANAGER
