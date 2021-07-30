#include "main_window.h"
#include "./ui_main_window.h"
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    this->system = Control::GetInstance(this);
    this->csd = new ConnectionSetupDialog();
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);
    QString sModule = "serverInteraction";
    Module* res = this->system->getModulesManager()->findModule(sModule.toStdString());
    serverInteraction = (UIModule*)res;

    if(serverInteraction == nullptr){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Failed to load serverInteraction module!");
        messageBox.setFixedSize(500,200);
        messageBox.show();
    } else{
        ui->serverTabGridLayout->addWidget(serverInteraction->getUI(""));
    }

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_connection_button_clicked()));


    //updateWsRunnerPropertiesUI(system->getWebSocketRunner()->);

}

MainWindow::~MainWindow(){
    delete ui;
    delete system;
}

void MainWindow::setup_connection(QString url, QString pawd){


}


void MainWindow::on_connection_button_clicked(){
    QString link = "ws://localhost:8080/control", pswd = "pswd";

    system->getWebSocketRunner()->setupConnection(link.toStdString(), pswd.toStdString());
    //csd->show();
    lastConnentionTime = QDateTime::currentDateTime();
    ui->label_lct_val->setText(this->lastConnentionTime.toString());
}


void MainWindow::updateWsRunnerPropertiesUI(wsr::WebSocketRunnerProperties wsRunnerProperties){
    this->wsrProperties = reformat(wsRunnerProperties);

    ui->label_ell_val->setText("all"); //this->wsrProperties.errors_logging_level
    ui->label_all_val->setText("all"); //this->wsrProperties.access_logging_level
    ui->label_wsr_myid_val->setText(this->wsrProperties.myID);
}

void MainWindow::updateConnectionMetainfoUI(wsr::ConnectionMetainfo connectionMetainfo){
    this->connectionMetainfo = reformat(connectionMetainfo);

    ui->id_val->setText(this->connectionMetainfo.ID);
    ui->mms_val->setText(this->connectionMetainfo.maxTransferringSize);
    ui->uri_val->setText(this->connectionMetainfo.uri);
    ui->status_val->setText(this->connectionMetainfo.status);
    ui->label_lec_val->setText(this->connectionMetainfo.lastErrorCode);
    ui->label_ler_val->setText(this->connectionMetainfo.lastErrorReason);
    ui->label_status_details_val->setText(this->connectionMetainfo.status_details);

/*    if(this->connectionMetainfo.status != "Connected"){
        ui->tabWidget->setCurrentIndex(0);
        QMessageBox messageBox;
        messageBox.warning(this, this->connectionMetainfo.status, this->connectionMetainfo.status_details);
        messageBox.setFixedSize(500,200);
        messageBox.show();
    }*/

}

void MainWindow::updateCommandsManagerPropertiesUI(cm::CommandsManagerProperties commandsManagerProperties){}
void MainWindow::updateModulesManagerPropertiesUI(mm::ModulesManagerProperties modulesManagerProperties){}


void MainWindow::showModule(QString moduleID, QString targetID){
    UIModule* module = (UIModule*)this->system->getModulesManager()->findModule(moduleID.toStdString());
    if(module == nullptr){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Failed to find mirror !");
        messageBox.setFixedSize(500,200);
        messageBox.show();
    } else{
         module->getUI(targetID)->show();
    }
}


ConnectionMetaInfo MainWindow::reformat(wsr::ConnectionMetainfo src){
    ConnectionMetaInfo temp;

    temp.closeStatusCode = QString::number(src.closeStatusCode);
    temp.ID = QString::fromStdString(src.myID);
    temp.lastErrorCode = QString::number(src.lastErrorCode.value());
    temp.lastErrorReason = QString::fromStdString(src.lastErrorReason);
    temp.maxTransferringSize = QString::number(src.maxTransferringSize);
    temp.status = QString::fromStdString(src.status);
    temp.status_details = QString::fromStdString(src.statusDetails);
    temp.uri = QString::fromStdString(src.uri);

    return temp;
}

WSRProperties MainWindow::reformat(wsr::WebSocketRunnerProperties src){
    WSRProperties temp;
    temp.access_logging_level = QString::number(src.accessLoggingLevel);
    temp.errors_logging_level = QString::number(src.errorsLoggingLevel);
    temp.myID = QString::fromStdString(src.myID);
    return temp;
}











