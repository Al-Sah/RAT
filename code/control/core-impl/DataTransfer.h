//
// Created by al_sah on 17.05.21.
//

#ifndef CORE_DATATRANSFER_H
#define CORE_DATATRANSFER_H

#include <QWidget>
#include "ApplicationContext.h"


class DataTransfer : public QWidget{
    Q_OBJECT
private:
    QWidget* target;

    signals:
    void wsRunnerPropertiesUpdated(wsr::WebSocketRunnerProperties ws_runner_properties);
    void connectionMetainfoUpdated(wsr::ConnectionMetainfo connectionMetainfo);
    void commandsManagerPropertiesUpdated(cm::CommandsManagerProperties commands_manager_properties);
    void modulesManagerPropertiesUpdated(mm::ModulesManagerProperties modules_manager_properties);

public:
    explicit DataTransfer(QWidget* target);
};
#endif //CORE_DATATRANSFER_H
