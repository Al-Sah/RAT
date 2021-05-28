//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_SYSTEM_H
#define BASIC_MODULE_SYSTEM_H

#include "configuration.h"
#include "WebsocketRunner.h"
#include "CommandsManager.h"
#include "ApplicationContext.h"
#include "ModulesManager.h"

class System {
private:
    std::shared_ptr<ApplicationContext> applicationContext;
    std::shared_ptr<CommandsManager> commandsManager;
    std::shared_ptr<ModulesManager> modulesManager;
    std::shared_ptr<WebsocketRunner> websocketRunner;

    std::function<bool(std::string)> message_sender;
    std::function<void(std::string)> message_register;

    std::function<void(TaskResult,ParsedTextMessage)> module_request_handler;
    std::function<void(TaskResult)> module_result_handler;

    std::function<void(std::string, std::string, std::shared_ptr<std::string>)> task_executor;

    std::function<void(wsr::ws_runner_properties &)> wsRunnerPropertiesUpdater;
    std::function<void(wsr::connection_metainfo &)> connectionMetainfoUpdater;
    //std::function<void(cm::commands_manager_properties &)> &commandsManagerPropertiesUpdater;
    //std::function<void(mm::modules_manager_properties &)> &modulesManagerPropertiesUpdater;


public:
    [[nodiscard]] const std::shared_ptr<ApplicationContext> &getApplicationContext() const;
    [[nodiscard]] const std::shared_ptr<CommandsManager> &getCommandsManager() const;
    [[nodiscard]] const std::shared_ptr<ModulesManager> &getModulesManager() const;
    [[nodiscard]] const std::shared_ptr<WebsocketRunner> &getWebsocketRunner() const;


    System(void * ui = nullptr);
};


#endif //BASIC_MODULE_SYSTEM_H