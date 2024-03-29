//
// Created by al_sah on 28.05.21.
//

#ifndef SERVER_INTERACTION_SERVERINTERACTION_H
#define SERVER_INTERACTION_SERVERINTERACTION_H

#include "../../UIModule.h"
#include "serverinteractiongroupbox.h"
#include "recourses.h"
#include <QObject>

class ServerInteraction : public UIModule{
    Q_OBJECT

private:
    QWidget* target;
    QWidget* moduleWindow;
    std::function<void(PayloadType, void*, void*)> callback;
    QString activeTargetID;

    TargetInfo parseTarget(std::string& line);
    QList<QString> parseTargetModules(std::string temp);
    QMap<QString, QString> parseTargetCharacteristics(std::string temp);
    void handleReceivedTargetsList(std::string& payload);

public:
    ServerInteraction(std::function<void(PayloadType, void*, void*)> &callback, void* data);
    void getTargetsList();
    void executeTask(std::string task,std::string payload, PayloadType pt, std::function<void(PayloadType, void*, bool)> callback) override;
    QWidget* getUI(QString targetId) override;

public slots:
    void do_targets_update();
    void showTargetModule(QListWidgetItem *);
    void setActiveTerget(QString id);

signals:
    void on_targets_update();
    void sendTargetsToUI(QList<TargetInfo> botsList, QList<TargetInfo> usersList);
    void showModule(QString moduleId, QString targetID);


};

#endif //SERVER_INTERACTION_SERVERINTERACTION_H
