#include "echodialogue.h"
#include "ui_echodialogue.h"
#include <QDateTime>

EchoDialogue::EchoDialogue(Echo *echo_ptr, QWidget *parent) : QDialog(parent), ui(new Ui::EchoDialogue){
    ui->setupUi(this);

    connect(ui->SendMesagge, SIGNAL(clicked(bool)), this, SLOT(sendMessageButtonClicked()));
    connect(ui->SendFile, SIGNAL(clicked(bool)), this, SLOT(sendFileButtonClicked()));
    //connect(echo_ptr, SIGNAL(openTarget(QString)), this, SLOT(openBot(QString)));
    connect(ui->botsList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showBot(QListWidgetItem *)));



}

EchoDialogue::~EchoDialogue(){
    delete ui;
}



void EchoDialogue::handleMessage(QString botId, QString text){

    QString oldText = botsDataList.value(botId).messages;
    QString newText = oldText.append("----New Message ---- ");
    QDateTime time = QDateTime::currentDateTime();
    newText = newText.append(time.toString()).append('\n').append(text).append('\n');
    botInfo inf;
    inf.messages = newText;
    QMap<QString, botInfo>::iterator i = botsDataList.find(botId);

    botsDataList.insert(i, botId, inf);
    ui->textBrowser->setText(inf.messages);
}

void EchoDialogue::handleFile(QString botId, QString filename){
    QString oldText = botsDataList.value(botId).messages;
    QString newText = oldText.append("----New File ---- ");
    QDateTime time = QDateTime::currentDateTime();
    newText = newText.append(time.toString()).append('\n').append(filename).append('\n');
    botInfo inf;
    inf.messages = newText;
    QMap<QString, botInfo>::iterator i = botsDataList.find(botId);
    botsDataList.insert(i, botId, inf);
    ui->textBrowser->setText(inf.messages);
}


void EchoDialogue::sendMessageButtonClicked(){
     emit sendUIMessage(activeBotId, ui->plainTextEdit->toPlainText());
}

void EchoDialogue::sendFileButtonClicked(){
    emit sendUIFile(activeBotId, "/home/al_sah/Desktop/rat_test.jpg");
}


void EchoDialogue::showBot(QListWidgetItem *item){
    ui->label_id_val->setText(item->text());
    botInfo info = botsDataList.value(item->text());
    ui->textBrowser->setText(info.messages);
}


void EchoDialogue::openBot(QString botId){

    activeBotId = botId;

    QMap<QString, botInfo>::const_iterator i = botsDataList.find(botId);
    if(i == botsDataList.end()){
        botInfo inf;
        botsDataList.insert(botId, inf);
        ui->botsList->addItem(botId);
    }
    botInfo info = botsDataList.value(botId);

    ui->textBrowser->setText(info.messages);
    ui->label_id_val->setText(botId);
}
