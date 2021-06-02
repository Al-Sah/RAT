#include "main_window.h"
#include "./ui_main_window.h"
#include <QPushButton>
#include <QMessageBox>
#include "test.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    this->system = new System(this);
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

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(on_conection_button_clicked()));


    updateWsRunnerPropertiesUI(system->getWebsocketRunner()->getProperties());

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::setup_connection(QString url, QString pawd){


}


void MainWindow::on_conection_button_clicked(){
    QString link = "ws://localhost:8080/control", pswd = "pswd";

    system->getWebsocketRunner()->setup_connection(link.toStdString(), pswd.toStdString());
    //csd->show();
    lastConnentionTime = QDateTime::currentDateTime();
    ui->label_lct_val->setText(this->lastConnentionTime.toString());
}


void MainWindow::updateWsRunnerPropertiesUI(wsr::ws_runner_properties wsRunnerProperties){
    this->wsrProperties = reformat(wsRunnerProperties);

    ui->label_ell_val->setText(this->wsrProperties.errors_logging_level);
    ui->label_all_val->setText(this->wsrProperties.access_logging_level);
    ui->label_wsr_myid_val->setText(this->wsrProperties.myID);
}

void MainWindow::updateConnectionMetainfoUI(wsr::connection_metainfo connectionMetainfo){
    this->connectionMetainfo = reformat(connectionMetainfo);

    ui->id_val->setText(this->connectionMetainfo.ID);
    ui->mms_val->setText(this->connectionMetainfo.maxTransferringSize);
    ui->uri_val->setText(this->connectionMetainfo.uri);
    ui->status_val->setText(this->connectionMetainfo.status);
    ui->label_lec_val->setText(this->connectionMetainfo.lastErrorCode);
    ui->label_ler_val->setText(this->connectionMetainfo.lastErrorReason);
    ui->label_status_details_val->setText(this->connectionMetainfo.status_details);

}

void MainWindow::updateCommandsManagerPropertiesUI(cm::commands_manager_properties commandsManagerProperties){}
void MainWindow::updateModulesManagerPropertiesUI(mm::modules_manager_properties modulesManagerProperties){}


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


ConnectionMetaInfo MainWindow::reformat(wsr::connection_metainfo src){
    ConnectionMetaInfo temp;

    temp.closeStatusCode = QString::number(src.closeStatusCode);
    temp.ID = QString::fromStdString(src.myID);
    temp.lastErrorCode = QString::number(src.last_error_code.value());
    temp.lastErrorReason = QString::fromStdString(src.last_error_reason);
    temp.maxTransferringSize = QString::number(src.max_transferring_size);
    temp.status = QString::fromStdString(src.status);
    temp.status_details = QString::fromStdString(src.status_details);
    temp.uri = QString::fromStdString(src.uri);

    return temp;
}

WSRProperties MainWindow::reformat(wsr::ws_runner_properties src){
    WSRProperties temp;
    temp.access_logging_level = QString::number(src.access_logging_level);
    temp.errors_logging_level = QString::number(src.errors_logging_level);
    temp.myID = QString::fromStdString(src.myID);
    return temp;
}











