#ifndef ECHODIALOGUE_H
#define ECHODIALOGUE_H

class PicturesTaker;
#include <QDialog>
#include <QListWidgetItem>
#include <QMap>
#include "resources.h"

namespace Ui {
class picturesTakerDialogue;
}

class PicturesTakerDialogue : public QDialog{
    Q_OBJECT

public:
    explicit PicturesTakerDialogue(PicturesTaker *echo_ptr, QWidget *parent = nullptr);
    ~PicturesTakerDialogue();

private:
    QMap<QString, botInfo> botsDataList;
    Ui::picturesTakerDialogue *ui;
    QString activeBotId;
    void showPictures(botInfo inf);

public slots:
    void handleFile(QString botId, QString path, QString pType);
    void UpdateCameraButtonClicked();
    void UpdateScreenshotButtonClicked();

    void openBot(QString botId);
    void showBot(QListWidgetItem *item);

signals:
    void getPicture(QString botId, QString type);
};

#endif // ECHODIALOGUE_H
