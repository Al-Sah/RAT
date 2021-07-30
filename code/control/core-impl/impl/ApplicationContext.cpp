//
// Created by al_sah on 18.07.21.
//

#include "../ApplicationContext.h"

ApplicationContext::ApplicationContext(std::string moduleId, std::string version, QWidget *parent): AbstractApplicationContext(moduleId, version) {
    this->run = true;
    this->dataTransfer = new DataTransfer(parent);
    this->notificator = std::thread(&ApplicationContext::send_data, this);
}

void ApplicationContext::send_data() {
    while(run){
        sleep(1);
        emit dataTransfer->wsRunnerPropertiesUpdated(webSocketRunnerProperties);
        emit dataTransfer->connectionMetainfoUpdated(connectionMetainfo);
        emit dataTransfer->modulesManagerPropertiesUpdated(modulesManagerProperties);
        emit dataTransfer->commandsManagerPropertiesUpdated(commandsManagerProperties);
    }
}

ApplicationContext::~ApplicationContext() {
    this->run = false;
    this->notificator.join();
    delete dataTransfer;
}