//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_SYSTEM_H
#define BASIC_MODULE_SYSTEM_H


#include "WebsocketRunner.h"
#include "CommandsManager.h"
#include "ApplicationContext.h"
#include "ModulesManager.h"
#include <thread>

#ifndef no_headers_includes
#define no_headers_includes
#endif
class System {
private:

    std::shared_ptr<ApplicationContext> applicationContext;
    std::shared_ptr<CommandsManager> commandsManager;
    std::shared_ptr<ModulesManager> modulesManager;
    std::shared_ptr<WebsocketRunner> websocketRunner;

    std::function<bool(std::string)> message_sender;
    std::function<void(std::string)> message_register;
    std::function<void(TaskResult)> module_result_handler;
    std::function<void(std::string, std::string, std::shared_ptr<std::string>)> task_executor;

public:
    System();
    [[noreturn]] void run();
};



#endif //BASIC_MODULE_SYSTEM_H
