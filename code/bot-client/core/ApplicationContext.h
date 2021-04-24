//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_APPLICATION_CONTEXT_H
#define BASIC_MODULE_APPLICATION_CONTEXT_H


#include "resources.h"

class ApplicationContext {

private:
    WSRunnerProperties wsRunnerProperties;
    CommandsManagerProperties commandsManagerProperties;
    ModulesManagerProperties modulesManagerProperties;

public:
    [[nodiscard]] const ModulesManagerProperties &getModulesManagerProperties() const;
    [[nodiscard]] const WSRunnerProperties &getWsRunnerProperties() const;
    [[nodiscard]] const CommandsManagerProperties &getCommandsManagerProperties() const;

    ApplicationContext();
};


#endif //BASIC_MODULE_APPLICATION_CONTEXT_H
