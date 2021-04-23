//
// Created by al_sah on 09.03.21.
//

#include "../System.h"

#include <memory>

System::System(){
    //std::shared_ptr<ApplicationContext>(new ApplicationContext());
    this->applicationContext = std::make_shared<ApplicationContext>();
    this->modulesManager  = std::make_shared<ModulesManager>();
    this->commandsManager = std::make_shared<CommandsManager>(applicationContext->getCommandsManagerProperties(), modulesManager);

    this->websocketRunner = std::make_unique<WebsocketRunner>(applicationContext->getWsRunnerProperties(), commandsManager);


    //CommandsManager *commandsManager;
    //ModulesManager *modulesManager;
    //this->websocketRunner = new WebsocketRunner();
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
