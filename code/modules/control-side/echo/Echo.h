#ifndef ECHO_H
#define ECHO_H

#include "../Module.h"
#include <QObject>


class Echo: public UIModule{
    Q_OBJECT
private:
    QWidget* target;
    QWidget* moduleWindow;
    std::function<void(payload_type, void*, void*)> callback;
    std::list<std::string> tasks;

public:
    Echo(std::function<void(payload_type, void*, void*)> &callback, void* data);
    void executeTask(std::string task, std::string payload, payload_type pt, std::function<void(payload_type, void*, bool)> callback) override;
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
