//
// Created by al_sah on 09.03.21.
//

#include "../AppContext/AbstractApplicationContext.h"


wsr::WebSocketRunnerProperties&  AbstractApplicationContext::getWsRunnerProperties() {
    return webSocketRunnerProperties;
}

cm::CommandsManagerProperties& AbstractApplicationContext::getTasksManagerProperties() {
    return commandsManagerProperties;
}

wsr::ConnectionMetainfo& AbstractApplicationContext::getConnectionMetainfo() {
    return connectionMetainfo;
}

mm::ModulesManagerProperties& AbstractApplicationContext::getModulesManagerProperties() {
    return modulesManagerProperties;
}

AbstractApplicationContext::AbstractApplicationContext(std::string &moduleId, std::string &version) : Module(moduleId, version) {}
