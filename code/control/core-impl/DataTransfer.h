//
// Created by al_sah on 17.05.21.
//

#ifndef CORE_DATATRANSFER_H
#define CORE_DATATRANSFER_H

#include <QWidget>
#include <TaskManaging/TaskManagerResources.h>
#include <ModulesManager/ModulesManagerResources.h>
#include <WebSocket/WebSocketResources.h>


class DataTransfer : public QWidget{
    Q_OBJECT
private:
    QWidget* target;

    signals:
    void wsRunnerPropertiesUpdated(wsr::WebSocketRunnerProperties webSocketRunnerProperties);
    void connectionMetainfoUpdated(wsr::ConnectionMetainfo connectionMetainfo);
    void commandsManagerPropertiesUpdated(cm::CommandsManagerProperties commandsManagerProperties);
    void modulesManagerPropertiesUpdated(mm::ModulesManagerProperties modulesManagerProperties);

public:
    explicit DataTransfer(QWidget* target);
};
#endif //CORE_DATATRANSFER_H
