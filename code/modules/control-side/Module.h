//
// Created by al_sah on 01.04.21.
//

#ifndef UIMODULE_BOT_MODULE_H
#define UIMODULE_BOT_MODULE_H

#include <QObject>
#include <QWidget>
#include "../Module.h"

class UIModule : public QObject, public Module{
    Q_OBJECT
public:
    virtual QWidget* getUI() = 0;
};


#endif //UIMODULE_BOT_MODULE_H
