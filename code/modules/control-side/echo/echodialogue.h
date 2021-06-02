#ifndef ECHODIALOGUE_H
#define ECHODIALOGUE_H

class Echo;
#include <QDialog>
#include <QListWidgetItem>
#include <QMap>
#include "resources.h"

namespace Ui {
class EchoDialogue;
}

class EchoDialogue : public QDialog{
    Q_OBJECT

public:
    explicit EchoDialogue(Echo *echo_ptr, QWidget *parent = nullptr);
    ~EchoDialogue();

private:
    QMap<QString, botInfo> botsDataList;
    Ui::EchoDialogue *ui;
    QString activeBotId;

public slots:
    void handleMessage(QString botId, QString text);
    void handleFile(QString botId, QString filename);
    void sendMessageButtonClicked();
    void sendFileButtonClicked();

    void openBot(QString botId);
    void showBot(QListWidgetItem *item);

signals:

    void sendUIMessage(QString botId, QString text);
    void sendUIFile(QString botId, QString path);
};

#endif // ECHODIALOGUE_H
