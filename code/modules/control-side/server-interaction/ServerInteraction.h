//
// Created by al_sah on 28.05.21.
//

#ifndef SERVER_INTERACTION_SERVERINTERACTION_H
#define SERVER_INTERACTION_SERVERINTERACTION_H

#include "../Module.h"
#include "serverinteractiongroupbox.h"
#include "recourses.h"
#include <QObject>

class ServerInteraction : public UIModule{
    Q_OBJECT

private:
    QWidget* target;
    QWidget* moduleWindow;
    std::function<void(payload_type, void*, void*)> callback;

    TargetInfo parseTarget(std::string& line);
    QList<QString> parseTargetModules(std::string temp);
    QMap<QString, QString> parseTargetCharacteristics(std::string temp);
    void handleReceivedTargetsList(std::string& payload);

public:
    ServerInteraction(std::function<void(payload_type, void*, void*)> &callback, void* data);
    void getTargetsList();
    void executeTask(std::string payload, payload_type pt, std::function<void(payload_type, void*, bool)> callback) override;
    QWidget* getUI() override;

public slots:
    void do_targets_update();
    void showTargetModule(QListWidgetItem *);

signals:
    void on_targets_update();
    void sendTargetsToUI(QList<TargetInfo> botsList, QList<TargetInfo> usersList);
    void showModule(QString);


};

#endif //SERVER_INTERACTION_SERVERINTERACTION_H
