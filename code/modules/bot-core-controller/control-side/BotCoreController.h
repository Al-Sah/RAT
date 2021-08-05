//
// Created by al_sah on 05.08.21.
//

#ifndef BOT_BOT_CORE_CONTROLLER
#define BOT_BOT_CORE_CONTROLLER

#include "../../UIModule.h"
#include <QObject>

class BotCoreController: public UIModule{
    Q_OBJECT
private:
    std::function<void(PayloadType, void*, void*)> callback;
    std::map<std::string,std::string> tasks;
    QWidget* moduleWindow;

public:
    BotCoreController(std::function<void(PayloadType, void*, void*)> &callback, void* data);
    void executeTask(std::string task, std::string payload, PayloadType pt, std::function<void(PayloadType, void*, bool)> callback) override;
    QWidget* getUI(QString targetId) override;
};


#endif //BOT_BOT_CORE_CONTROLLER
