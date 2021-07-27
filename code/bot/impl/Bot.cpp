//
// Created by al_sah on 17.07.21.
//

#include "../Bot.h"
#include <csignal>

Bot* Bot::bot = nullptr;
volatile bool Bot::run = true;


Bot::Bot() {
    this->moduleId = "RatBotCore";
    this->version = "1.0.0";

    this->context = std::make_shared<ApplicationContext>("BotApplicationContext", "1.0.0");
    this->modulesManager = std::make_shared<ModulesManager>("BotModulesManager", "1.0.0", context->getModulesManagerProperties());
    this->tasksManager = std::make_shared<TaskManager>("BotTaskManager", "1.0.0", context->getTasksManagerProperties());
    this->webSocketRunner =  std::make_shared<WebSocketRunner>
            ("BotWebSocketRunner", "1.0.0", context->getWsRunnerProperties(), context->getConnectionMetainfo());

    webSocketRunner->setTaskRegister(tasksManager.get());

    tasksManager->setTaskExecutor(modulesManager.get());
    tasksManager->setMessagesSender(webSocketRunner.get());

    modulesManager->setTaskResultRegister(tasksManager.get());
    modulesManager->registerModule(this);
    modulesManager->loadExternalModules();

    signal(SIGQUIT, signalHandler);
    signal(SIGTERM, signalHandler);
}


void Bot::signalHandler(int signal) {
    std::cout << "Interrupt signal (" << signal << ") received.\n";
    Bot::run = false;
}

Bot* Bot::GetInstance() {

    if(bot == nullptr){
        bot = new Bot();
    }
    return bot;
}

const std::shared_ptr<ApplicationContext> &Bot::getApplicationContext() const {
    return context;
}

const std::shared_ptr<TaskManager> &Bot::getCommandsManager() const {
    return tasksManager;
}

const std::shared_ptr<ModulesManager> &Bot::getModulesManager() const {
    return modulesManager;
}

const std::shared_ptr<WebSocketRunner> &Bot::getWebSocketRunner() const {
    return webSocketRunner;
}

void Bot::runPerpetual() {


    bot->getWebSocketRunner()->setupConnection("ws://localhost:8080/bot");
    while (Bot::run){
        sleep(1);
    }
    bot->getCommandsManager()->stop_work();
    bot->getWebSocketRunner()->closeConnection(websocketpp::close::status::normal, "Bot is going away");
}

void Bot::executeTask(std::string task, std::string payload, PayloadType pt, std::function<void(PayloadType, void *, bool)> callback) {

    std::string result = modulesManager->getModules();
    callback(PayloadType::text, &result, true);
}

void Bot::sendExternalModules() {

}

