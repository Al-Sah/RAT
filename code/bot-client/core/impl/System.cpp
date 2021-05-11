//
// Created by al_sah on 09.03.21.
//

#include "../System.h"

#include <memory>

System::System(){

    this->applicationContext = std::make_shared<ApplicationContext>();
    this->modulesManager  = std::make_shared<ModulesManager> (applicationContext->getModulesManagerProperties());
    this->commandsManager = std::make_shared<CommandsManager>(applicationContext->getCommandsManagerProperties());
    this->websocketRunner = std::make_shared<WebsocketRunner>(applicationContext->getWsRunnerProperties());

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
    this->module_result_handler = [this](TaskResult message){
        return this->commandsManager->register_result_message(message);
    };

    this->commandsManager->setMessageSender(message_sender);
    this->commandsManager->setTaskExecutor(task_executor);

    this->websocketRunner->set_messages_register(message_register);
    this->modulesManager->set_result_handler(module_result_handler);

#endif
}

[[noreturn]] void System::run() {
    std::cout << "Hello, World!" << std::endl;

    websocketRunner->setup_connection("ws://localhost:8080/bot");

    std::string message;
/*    while (true){
        getline(std::cin, message);
        if(message == "break"){
            break;
        } else if(message == "info"){
            auto cm = websocketRunner->getConnectionMetainfo();
            std::cout  << "\nconnection_hdl: "<< cm.hdl.lock()
                       << "\nStatus: " << cm.status
                       << "\nuri: " << cm.uri
                       << "\nlastErrorCode: " << cm.lastErrorCode
                       << "\nlastErrorReason: " << cm.lastErrorReason << std::endl;
            continue;
        }
        websocketRunner->send_message(message);
    }*/

    while(true){
        sleep(100);
    }

    websocketRunner->close_connection();
}
