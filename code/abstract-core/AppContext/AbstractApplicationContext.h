//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_APPLICATION_CONTEXT_H
#define BASIC_MODULE_APPLICATION_CONTEXT_H

#include "../TaskManaging/TaskManagerResources.h"
#include "../ModulesManager/ModulesManagerResources.h"
#include "../WebSocket/WebSocketResources.h"


class AbstractApplicationContext : public Module{

protected:
    wsr::WebSocketRunnerProperties webSocketRunnerProperties;
    wsr::ConnectionMetainfo connectionMetainfo;
    cm::CommandsManagerProperties commandsManagerProperties;
    mm::ModulesManagerProperties modulesManagerProperties;

public:

    [[nodiscard]] mm::ModulesManagerProperties& getModulesManagerProperties();
    [[nodiscard]] wsr::ConnectionMetainfo& getConnectionMetainfo();
    [[nodiscard]] wsr::WebSocketRunnerProperties& getWsRunnerProperties();
    [[nodiscard]] cm::CommandsManagerProperties& getTasksManagerProperties();

    AbstractApplicationContext(std::string &moduleId, std::string &version);
};

#endif //BASIC_MODULE_APPLICATION_CONTEXT_H