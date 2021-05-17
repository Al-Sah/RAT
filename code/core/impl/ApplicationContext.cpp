//
// Created by al_sah on 09.03.21.
//

#include "../ApplicationContext.h"

ApplicationContext::ApplicationContext(void * ui){
    this->module_id = "ApplicationContext";
#ifdef CONTROL_ENABLE
    this->dataTransfer = new DataTransfer( (QObject*)ui);
#endif
}

const wsr::ws_runner_properties &ApplicationContext::getWsRunnerProperties() const {
    return ws_runner_properties;
}

const cm::commands_manager_properties  &ApplicationContext::getCommandsManagerProperties() const {
    return commands_manager_properties;
}

const mm::modules_manager_properties &ApplicationContext::getModulesManagerProperties() const {
    return modules_manager_properties;
}

void ApplicationContext::executeTask(std::string payload, payload_type pt, std::function<void(payload_type, void *, bool)> callback) {

}
void ApplicationContext::setWsRunnerProperties(const wsr::ws_runner_properties &wsRunnerProperties) {
    ws_runner_properties = wsRunnerProperties;
}
void ApplicationContext::setConnectionMetainfo(const wsr::connection_metainfo &connectionMetainfo) {
    ApplicationContext::connectionMetainfo = connectionMetainfo;
}
void ApplicationContext::setCommandsManagerProperties(const cm::commands_manager_properties &commandsManagerProperties) {
    commands_manager_properties = commandsManagerProperties;
}
void ApplicationContext::setModulesManagerProperties(const mm::modules_manager_properties &modulesManagerProperties) {

}
const wsr::connection_metainfo &ApplicationContext::getConnectionMetainfo() const {
    return connectionMetainfo;
}



/*void ApplicationContext::setWsRunnerPropertiesUpdater(
        const std::function<void(wsr::ws_runner_properties &)> &wsRunnerPropertiesUpdater) {
    ws_runner_properties_updater = wsRunnerPropertiesUpdater;
}

void ApplicationContext::setConnectionMetainfoUpdater(
        const std::function<void(wsr::connection_metainfo &)> &connectionMetainfoUpdater) {
    connection_metainfo_updater = connectionMetainfoUpdater;
}

void ApplicationContext::setCommandsManagerPropertiesUpdater(
        const std::function<void(cm::commands_manager_properties &)> &commandsManagerPropertiesUpdater) {
    commands_manager_properties_updater = commandsManagerPropertiesUpdater;
}

void ApplicationContext::setModulesManagerPropertiesUpdater(
        const std::function<void(mm::modules_manager_properties &)> &modulesManagerPropertiesUpdater) {
    modules_manager_properties_updater = modulesManagerPropertiesUpdater;
}*/



void ApplicationContext::updateConnectionMetainfo(const wsr::connection_metainfo &connectionMetainfo) {
    setConnectionMetainfo(connectionMetainfo);
    #ifdef CONTROL_ENABLE
    emit dataTransfer->connectionMetainfoUpdated(connectionMetainfo);
    #endif
}

void ApplicationContext::updateWsRunnerProperties(const wsr::ws_runner_properties &wsRunnerProperties) {
    setWsRunnerProperties(wsRunnerProperties);
    #ifdef CONTROL_ENABLE
    emit dataTransfer->wsRunnerPropertiesUpdated(wsRunnerProperties);
    #endif
}

void ApplicationContext::updateCommandsManagerProperties(const cm::commands_manager_properties &commandsManagerProperties) {
    setCommandsManagerProperties(commandsManagerProperties);
    #ifdef CONTROL_ENABLE
    emit dataTransfer->commandsManagerPropertiesUpdated(commandsManagerProperties);
    #endif
}

void ApplicationContext::updateModulesManagerProperties(const mm::modules_manager_properties &modulesManagerProperties) {
    setModulesManagerProperties(modulesManagerProperties);
    #ifdef CONTROL_ENABLE
    emit dataTransfer->modulesManagerPropertiesUpdated(modulesManagerProperties);
    #endif
}
