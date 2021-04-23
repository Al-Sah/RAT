//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_SYSTEM_H
#define BASIC_MODULE_SYSTEM_H


#include "WebsocketRunner.h"
#include "CommandsManager.h"
#include "ApplicationContext.h"
#include "ModulesManager.h"

class System {
private:
    std::shared_ptr<ApplicationContext> applicationContext;


    std::shared_ptr<CommandsManager> commandsManager;
    std::shared_ptr<ModulesManager> modulesManager;

    std::unique_ptr<WebsocketRunner> websocketRunner;



public:
    System();
    void run();
};


#endif //BASIC_MODULE_SYSTEM_H
