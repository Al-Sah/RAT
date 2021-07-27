//
// Created by al_sah on 22.07.21.
//

#include "../Control.h"

Control* Control::control = nullptr;
volatile bool Control::run = true;

Control::Control(QWidget *parent) {
    this->parent = parent;

    this->moduleId = "RatControlSide";
    this->version = "1.0.0";

    this->context = std::make_shared<ApplicationContext>
            ("ControlApplicationContext", "1.0.0");
    this->modulesManager = std::make_shared<ModulesManager>
            ("ControlModulesManager", "1.0.0", context->getModulesManagerProperties(), parent);
    this->tasksManager = std::make_shared<TaskManager>
            ("ControlTaskManager", "1.0.0", context->getTasksManagerProperties());
    this->webSocketRunner =  std::make_shared<WebSocketRunner>
            ("ControlWebSocketRunner", "1.0.0", context->getWsRunnerProperties(), context->getConnectionMetainfo());

    webSocketRunner->setTaskRegister(tasksManager.get());

    tasksManager->setTaskExecutor(modulesManager.get());
    tasksManager->setMessagesSender(webSocketRunner.get());

    modulesManager->setTaskResultRegister(tasksManager.get());


    modulesManager->loadExternalModules();
    modulesManager->registerModule(this);

}

const std::shared_ptr<ApplicationContext> &Control::getApplicationContext() const {
    return context;
}

const std::shared_ptr<TaskManager> &Control::getTasksManager() const {
    return tasksManager;
}

const std::shared_ptr<ModulesManager> &Control::getModulesManager() const {
    return modulesManager;
}

const std::shared_ptr<WebSocketRunner> &Control::getWebSocketRunner() const {
    return webSocketRunner;
}

Control *Control::GetInstance(QWidget *parent) {

    if(control == nullptr){
        control = new Control(parent);
    }
    return control;
}

void Control::executeTask(std::string task, std::string payload, PayloadType pt,
                          std::function<void(PayloadType, void *, bool)> callback) {

}

QWidget *Control::getUI(QString targetId) {
    return nullptr;
}
