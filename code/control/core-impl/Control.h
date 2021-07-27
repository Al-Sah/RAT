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
#include <QWidget>

class Control: public UIModule {

private:
    static volatile bool run;
    static Control* control;
    QWidget *parent;

    std::shared_ptr<ApplicationContext> context;
    std::shared_ptr<TaskManager> tasksManager;
    std::shared_ptr<ModulesManager> modulesManager;
    std::shared_ptr<WebSocketRunner> webSocketRunner;
    explicit Control(QWidget *parent);


public:

    void executeTask(std::string task, std::string payload, PayloadType pt, std::function<void(PayloadType, void *, bool)> callback) override;
    QWidget *getUI(QString targetId) override;

    [[nodiscard]] const std::shared_ptr<ApplicationContext> &getApplicationContext() const;
    [[nodiscard]] const std::shared_ptr<TaskManager> &getTasksManager() const;
    [[nodiscard]] const std::shared_ptr<ModulesManager> &getModulesManager() const;
    [[nodiscard]] const std::shared_ptr<WebSocketRunner> &getWebSocketRunner() const;

    Control(Control &other) = delete;
    void operator=(const Control &) = delete;
    static Control *GetInstance(QWidget *parent);
    ~Control() = default;

};


#endif //BOT_MAIN_CLASS