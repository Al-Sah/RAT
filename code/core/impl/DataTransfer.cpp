//
// Created by al_sah on 17.05.21.
//

#include "../DataTransfer.h"


DataTransfer::DataTransfer(QObject *target) {
    this->target = target;

    connect(this, SIGNAL(wsRunnerPropertiesUpdated(wsr::ws_runner_properties)),
            this->target, SLOT(updateWsRunnerPropertiesUI(wsr::ws_runner_properties)));

    connect(this, SIGNAL(connectionMetainfoUpdated(wsr::connection_metainfo)),
            this->target, SLOT(updateConnectionMetainfoUI(wsr::connection_metainfo)));

    connect(this, SIGNAL(commandsManagerPropertiesUpdated(cm::commands_manager_properties)),
            this->target, SLOT(updateCommandsManagerPropertiesUI(cm::commands_manager_properties)));

    connect(this, SIGNAL(modulesManagerPropertiesUpdated(mm::modules_manager_properties)),
            this->target, SLOT(updateModulesManagerPropertiesUI(mm::modules_manager_properties)));

}
