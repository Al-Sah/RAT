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

    commandsManager->setModulesManager(modulesManager);
    commandsManager->setWebsocketRunner(websocketRunner);

    websocketRunner->setCommandsManager(commandsManager);
    modulesManager ->setCommandsManager(commandsManager);


}

void System::run() {
    std::cout << "Hello, World!" << std::endl;

    websocketRunner->setup_connection("ws://localhost:8080/bot");

    std::string message;
    while (true){
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
    }

    websocketRunner->close_connection();
}
