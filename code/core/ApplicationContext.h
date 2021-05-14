//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_APPLICATION_CONTEXT_H
#define BASIC_MODULE_APPLICATION_CONTEXT_H


#include "cm_resources.h"
#include "mm_resources.h"
#include "wsr_resources.h"

class ApplicationContext : public Module{

private:
    wsr::ws_runner_properties ws_runner_properties;
    cm::commands_manager_properties commands_manager_properties;
    mm::modules_manager_properties modules_manager_properties;

public:
    [[nodiscard]] const mm::modules_manager_properties &getModulesManagerProperties() const;
    [[nodiscard]] const wsr::ws_runner_properties &getWsRunnerProperties() const;
    [[nodiscard]] const cm::commands_manager_properties &getCommandsManagerProperties() const;

    ApplicationContext();
    void executeTask(std::string payload, payload_type pt, std::function<void (payload_type, void *, bool)> callback) override;
};


#endif //BASIC_MODULE_APPLICATION_CONTEXT_H
