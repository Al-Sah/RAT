//
// Created by al_sah on 01.04.21.
//

#ifndef UIMODULE_BOT_MODULE_H
#define UIMODULE_BOT_MODULE_H


#include "GlobalResources.h"
#include <QObject>
#include <QWidget>
#include "Module.h"


class UIModule : public QObject, public Module {
    Q_OBJECT
public:
    virtual void executeTask(std::string task, std::string payload, PayloadType pt, std::function<void(PayloadType, void*, bool)> callback) = 0;
    virtual QWidget* getUI(QString targetId) = 0;
    //~UIModule() override = default;
};

using getUIModuleInstance = UIModule* (*)(std::function<void(PayloadType, void*, void*)> &callback, void* data);


#endif //UIMODULE_BOT_MODULE_H
