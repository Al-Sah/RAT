//
// Created by al_sah on 17.05.21.
//

#include "../DataTransfer.h"


DataTransfer::DataTransfer(QWidget *target) {
    this->target = target;

    connect(this, SIGNAL(wsRunnerPropertiesUpdated(wsr::WebSocketRunnerProperties)),
            this->target, SLOT(updateWsRunnerPropertiesUI(wsr::WebSocketRunnerProperties)));

    connect(this, SIGNAL(connectionMetainfoUpdated(wsr::ConnectionMetainfo)),
            this->target, SLOT(updateConnectionMetainfoUI(wsr::ConnectionMetainfo)));

    connect(this, SIGNAL(commandsManagerPropertiesUpdated(cm::CommandsManagerProperties)),
            this->target, SLOT(updateCommandsManagerPropertiesUI(cm::CommandsManagerProperties)));

    connect(this, SIGNAL(modulesManagerPropertiesUpdated(mm::ModulesManagerProperties)),
            this->target, SLOT(updateModulesManagerPropertiesUI(mm::ModulesManagerProperties)));
}