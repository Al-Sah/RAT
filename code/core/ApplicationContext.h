//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_APPLICATION_CONTEXT_H
#define BASIC_MODULE_APPLICATION_CONTEXT_H

#include "cm_resources.h"
#include "mm_resources.h"
#include "wsr_resources.h"

#ifdef CONTROL_ENABLE
#include "DataTransfer.h"
#endif


class ApplicationContext : public Module{

private:
    wsr::ws_runner_properties ws_runner_properties;
    wsr::connection_metainfo connectionMetainfo;
    cm::commands_manager_properties commands_manager_properties;
    mm::modules_manager_properties modules_manager_properties;

#ifdef CONTROL_ENABLE
    DataTransfer* dataTransfer;
#endif

public:

    void setWsRunnerProperties(const wsr::ws_runner_properties &wsRunnerProperties);
    void setConnectionMetainfo(const wsr::connection_metainfo &connectionMetainfo);
    void setCommandsManagerProperties(const cm::commands_manager_properties &commandsManagerProperties);
    void setModulesManagerProperties(const mm::modules_manager_properties &modulesManagerProperties);

    void updateWsRunnerProperties(const wsr::ws_runner_properties &wsRunnerProperties);
    void updateConnectionMetainfo(const wsr::connection_metainfo &connectionMetainfo);
    void updateCommandsManagerProperties(const cm::commands_manager_properties &commandsManagerProperties);
    void updateModulesManagerProperties(const mm::modules_manager_properties &modulesManagerProperties);


public:
    [[nodiscard]] const mm::modules_manager_properties &getModulesManagerProperties() const;
    [[nodiscard]] const wsr::connection_metainfo &getConnectionMetainfo() const;
    [[nodiscard]] const wsr::ws_runner_properties &getWsRunnerProperties() const;
    [[nodiscard]] const cm::commands_manager_properties &getCommandsManagerProperties() const;

    explicit ApplicationContext(void * ui);
    void executeTask(std::string payload, payload_type pt, std::function<void (payload_type, void *, bool)> callback) override;
};


#endif //BASIC_MODULE_APPLICATION_CONTEXT_H
