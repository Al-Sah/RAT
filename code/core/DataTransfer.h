//
// Created by al_sah on 17.05.21.
//

#ifndef CORE_DATATRANSFER_H
#define CORE_DATATRANSFER_H

#include <QWidget>
#include "cm_resources.h"
#include "mm_resources.h"
#include "wsr_resources.h"


class DataTransfer : public QWidget{
    Q_OBJECT
private:
    QObject* target;

    signals:
    void wsRunnerPropertiesUpdated(wsr::ws_runner_properties ws_runner_properties);
    void connectionMetainfoUpdated(wsr::connection_metainfo connectionMetainfo);
    void commandsManagerPropertiesUpdated(cm::commands_manager_properties commands_manager_properties);
    void modulesManagerPropertiesUpdated(mm::modules_manager_properties modules_manager_properties);

public:
    explicit DataTransfer(QObject* target);
};
#endif //CORE_DATATRANSFER_H
