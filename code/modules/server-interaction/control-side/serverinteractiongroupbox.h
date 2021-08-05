#ifndef SERVERINTERACTIONGROUPBOX_H
#define SERVERINTERACTIONGROUPBOX_H

class ServerInteraction;
#include <QGroupBox>
#include <QListWidgetItem>
#include "recourses.h"

namespace Ui {
class ServerInteractionGroupBox;
}


class ServerInteractionGroupBox : public QGroupBox{
    Q_OBJECT

private:
    Ui::ServerInteractionGroupBox *ui;
    ServerInteraction *si_ptr;

    QList<TargetInfo> bots;
    QList<TargetInfo> users;

    void updateTargetUI(TargetInfo& info);

public:
    explicit ServerInteractionGroupBox(ServerInteraction *si_ptr, QWidget *parent = nullptr);
    ~ServerInteractionGroupBox();



public slots:
    //void updateTargetsRequest();
    void handleUpdatedTargets(QList<TargetInfo>, QList<TargetInfo>);
    void showTargetInfo(QListWidgetItem *);

signals:
    void setActiveTerget(QString id);


};

#endif // SERVERINTERACTIONGROUPBOX_H
