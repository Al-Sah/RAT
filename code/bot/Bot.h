//
// Created by al_sah on 17.07.21.
//

#ifndef BOT_MAIN_CLASS
#define BOT_MAIN_CLASS

//#include "Configuration/configuration.h"
#include "WebSocketRunner.h"
#include "TaskManager.h"
#include "ApplicationContext.h"
#include "ModulesManager.h"

class Bot: public TaskExecutor {

private:
    static volatile bool run;
    static Bot* bot;

    std::shared_ptr<ApplicationContext> context;
    std::shared_ptr<TaskManager> tasksManager;
    std::shared_ptr<ModulesManager> modulesManager;
    std::shared_ptr<WebSocketRunner> webSocketRunner;

    static void signalHandler(int signal);
    Bot();

    void sendExternalModules();

public:
    const std::shared_ptr<ApplicationContext> &getApplicationContext() const;
    const std::shared_ptr<TaskManager> &getCommandsManager() const;
    const std::shared_ptr<ModulesManager> &getModulesManager() const;
    const std::shared_ptr<WebSocketRunner> &getWebSocketRunner() const;

    Bot(Bot &other) = delete;
    void operator=(const Bot &) = delete;
    static Bot *GetInstance();

    void runPerpetual();

    void executeTask(std::string task, std::string payload, PayloadType pt, std::function<void(PayloadType, void *, bool)> callback) override;
};

#endif //BOT_MAIN_CLASS