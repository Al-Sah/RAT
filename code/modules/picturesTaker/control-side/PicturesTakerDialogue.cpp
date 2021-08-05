#include "PicturesTakerDialogue.h"
#include "ui_PicturesTakerDialogue.h"
#include <QDateTime>
#include <QImageReader>
#include <QMessageBox>
#include <QDir>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

PicturesTakerDialogue::PicturesTakerDialogue(PicturesTaker *echo_ptr, QWidget *parent) : QDialog(parent), ui(new Ui::picturesTakerDialogue){
    ui->setupUi(this);

    connect(ui->UpdateCameraButton, SIGNAL(clicked(bool)), this, SLOT(UpdateCameraButtonClicked()));
    connect(ui->UpdateScreenshotButton, SIGNAL(clicked(bool)), this, SLOT(UpdateScreenshotButtonClicked()));
    connect(ui->botsList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showBot(QListWidgetItem *)));
}

PicturesTakerDialogue::~PicturesTakerDialogue(){
    delete ui;
}

void PicturesTakerDialogue::handleFile(QString botId, QString path, QString pType){

    botInfo inf;
    if(pType == "camera"){
        inf.screenshotPath = botsDataList.value(botId).screenshotPath;
        inf.cameraPhotoPath = path;
    } else{
        inf.cameraPhotoPath = botsDataList.value(botId).cameraPhotoPath;
        inf.screenshotPath = path;
    }

    QMap<QString, botInfo>::iterator i = botsDataList.find(botId);
    botsDataList.insert(i, botId, inf);

    if(botId == activeBotId){
        ui->label_id_val->setText(botId);
        showPictures(inf);
    }
}

void PicturesTakerDialogue::showPictures(botInfo inf){
    QPixmap camera = QPixmap(inf.cameraPhotoPath);

    /*if(!camera.isNull()){
        ui->label_camera->setPixmap(camera.scaled( ui->label_camera->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    } else{
        ui->label_camera->setText("Undefined");
    }*/

    /*QPixmap screen = QPixmap(inf.screenshotPath);
    if(!screen.isNull()){
        ui->label_screen->setPixmap(screen.scaled( ui->label_screen->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    } else{
        ui->label_screen->setText("Undefined");
    }*/

    QImageReader cameraReader(inf.cameraPhotoPath);
    cameraReader.setAutoTransform(true);
    QImage cameraImage;
    cameraImage = cameraReader.read();
    if (cameraImage.isNull()) {
        //ui->label_camera->setText("Undefined");
    } else{
        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(cameraImage));
        scene->addItem(item);
        ui->cameraView->setScene(scene);
    }

    QImageReader screenReader(inf.screenshotPath);
    screenReader.setAutoTransform(true);
    QImage screenImage;
    screenImage = screenReader.read();
    if (!screenImage.isNull()) {
        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(screenImage));
        scene->addItem(item);
        ui->screenView->setScene(scene);
    }

}


void PicturesTakerDialogue::UpdateCameraButtonClicked(){
     emit getPicture(activeBotId, "camera");
}

void PicturesTakerDialogue::UpdateScreenshotButtonClicked(){
    emit getPicture(activeBotId, "screenshot");
}


void PicturesTakerDialogue::showBot(QListWidgetItem *item){
    ui->label_id_val->setText(item->text());
    activeBotId= item->text();
    showPictures(botsDataList.value(item->text()));
}


void PicturesTakerDialogue::openBot(QString botId){

    activeBotId = botId;

    QMap<QString, botInfo>::const_iterator i = botsDataList.find(botId);
    if(i == botsDataList.end()){
        botInfo inf;
        botsDataList.insert(botId, inf);
        ui->botsList->addItem(botId);
    }
    botInfo info = botsDataList.value(botId);
    ui->label_id_val->setText(botId);
    showPictures(info);
}
