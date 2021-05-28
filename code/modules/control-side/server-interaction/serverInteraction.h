//
// Created by al_sah on 28.05.21.
//

#ifndef SERVER_INTERACTION_SERVERINTERACTION_H
#define SERVER_INTERACTION_SERVERINTERACTION_H

#include "../../Module.h"
#include <QWidget>

class serverInteraction : public QObject, public Module{
    Q_OBJECT

private:
    QObject* target;
    std::function<void(payload_type, void*, void*)> callback;

public:
    serverInteraction(std::function<void(payload_type, void*, void*)> &callback, void* data);
    void getTargetsList();
    void executeTask(std::string payload, payload_type pt, std::function<void(payload_type, void*, bool)> callback) override;

public slots:
    void do_targets_update();
};

#endif //SERVER_INTERACTION_SERVERINTERACTION_H
