//
// Created by al_sah on 09.03.21.
//

#include "../System.h"
#include <memory>


System::System(void * ui){

    this->applicationContext = std::make_shared<ApplicationContext>(ui);
    this->modulesManager  = std::make_shared<ModulesManager> (applicationContext->getModulesManagerProperties(), ui);
    this->commandsManager = std::make_shared<CommandsManager>(applicationContext->getCommandsManagerProperties());
    this->websocketRunner = std::make_shared<WebsocketRunner>(applicationContext->getWsRunnerProperties());

    modulesManager->registerModule(commandsManager.get());
    modulesManager->registerModule(websocketRunner.get());

#ifdef headers_includes
    commandsManager->setModulesManager(modulesManager);
    commandsManager->setWebsocketRunner(websocketRunner);
    websocketRunner->setCommandsManager(commandsManager);
    modulesManager->setCommandsManager(commandsManager);
#else

    this->message_sender = [this](const std::string& message){
        return this->websocketRunner->send_message(message);
    };
    this->message_register = [this](std::string message){
        return this->commandsManager->register_inbox_message(message);
    };

    this->task_executor = [this](std::string module, std::string task_id, std::shared_ptr<std::string> payload){
        return this->modulesManager->handleTask(module, task_id, payload);
    };
    this->module_request_handler = [this](TaskResult message, ParsedTextMessage parsedMessage){
        return this->commandsManager->register_result_message(message, parsedMessage);
    };

#ifdef CONTROL_ENABLE
/*    this->commandsManagerPropertiesUpdater = [this](auto properties){
        this->applicationContext->updateCommandsManagerProperties(properties);
    };*/

    this->connectionMetainfoUpdater = [this](auto info){
        this->applicationContext->updateConnectionMetainfo(info);
    };

/*    this->modulesManagerPropertiesUpdater = [this](auto properties){
        this->applicationContext->updateModulesManagerProperties(properties);
    };*/

    this->wsRunnerPropertiesUpdater = [this](auto properties){
        this->applicationContext->updateWsRunnerProperties (properties);
    };

    this->websocketRunner->setWsRunnerPropertiesUpdater(wsRunnerPropertiesUpdater);
    this->websocketRunner->setConnectionMetainfoUpdater(connectionMetainfoUpdater);
#endif

    this->commandsManager->setMessageSender(message_sender);
    this->commandsManager->setTaskExecutor(task_executor);
    this->websocketRunner->set_messages_register(message_register);
    this->modulesManager->set_result_handler(module_request_handler);

#endif
}

const std::shared_ptr<ApplicationContext> &System::getApplicationContext() const {
    return applicationContext;
}

const std::shared_ptr<CommandsManager> &System::getCommandsManager() const {
    return commandsManager;
}

const std::shared_ptr<ModulesManager> &System::getModulesManager() const {
    return modulesManager;
}

const std::shared_ptr<WebsocketRunner> &System::getWebsocketRunner() const {
    return websocketRunner;
}