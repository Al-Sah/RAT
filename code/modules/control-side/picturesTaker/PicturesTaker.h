#ifndef PicturesTaker_H
#define PicturesTaker_H

#include "../Module.h"
#include <QObject>


class PicturesTaker: public UIModule{
    Q_OBJECT
private:
    QWidget* target;
    QWidget* moduleWindow;
    std::function<void(payload_type, void*, void*)> callback;
    std::map<std::string, std::pair<std::string, std::string>> tasks; // task, bot_id, type

public:
    PicturesTaker(std::function<void(payload_type, void*, void*)> &callback, void* data);
    void executeTask(std::string task, std::string payload, payload_type pt, std::function<void(payload_type, void*, bool)> callback) override;
    QWidget* getUI(QString targetId) override;

public slots:
    void sendRequest(QString botId, QString text);

signals:
    void onResult(QString botId, QString path, QString type);
    void openTarget(QString botId);
};

#endif // PicturesTaker_H
