//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_APPLICATION_CONTEXT_H
#define BASIC_MODULE_APPLICATION_CONTEXT_H


#include "resources.h"
#include "modules/Module.h"

class ApplicationContext : public Module{

private:
    WSRunnerProperties wsRunnerProperties;
    CommandsManagerProperties commandsManagerProperties;
    ModulesManagerProperties modulesManagerProperties;

public:
    [[nodiscard]] const ModulesManagerProperties &getModulesManagerProperties() const;
    [[nodiscard]] const WSRunnerProperties &getWsRunnerProperties() const;
    [[nodiscard]] const CommandsManagerProperties &getCommandsManagerProperties() const;

    ApplicationContext();
    void executeTask(std::string payload, payload_type pt, std::function<void (payload_type, void *, bool)> callback) override;
};


#endif //BASIC_MODULE_APPLICATION_CONTEXT_H
