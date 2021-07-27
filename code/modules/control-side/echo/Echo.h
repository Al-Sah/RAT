#ifndef ECHO_H
#define ECHO_H

#include "../../UIModule.h"
#include <QObject>


class Echo: public UIModule{
    Q_OBJECT
private:
    QWidget* target;
    QWidget* moduleWindow;
    std::function<void(PayloadType, void*, void*)> callback;
    std::map<std::string,std::string> tasks;

public:
    Echo(std::function<void(PayloadType, void*, void*)> &callback, void* data);
    void executeTask(std::string task, std::string payload, PayloadType pt, std::function<void(PayloadType, void*, bool)> callback) override;
    QWidget* getUI(QString targetId) override;


public slots:
    void sendMessage(QString botId, QString text);
    void sendFile(QString botId, QString path);

signals:
    void onMessage(QString botId, QString text);
    void onFile(QString botId, QString text);
    void openTarget(QString botId);
};

#endif // ECHO_H
