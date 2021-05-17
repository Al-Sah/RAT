#include "main_window.h"
#include "./ui_main_window.h"
#include <QPushButton>
#include "test.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){


   this->system = new System(this);
   this->csd = new ConnectionSetupDialog();
   ui->setupUi(this);

   getSomeLayout(ui->gridLayout, this);

   //ui->gridLayout->addWidget(new QPushButton("OK"), 0, 0);
   //ui->gridLayout->addWidget(new QPushButton("OK", this), 1, 1);
   //ui->gridLayout->addWidget(new QPushButton("OK", this), 2, 2);

   connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(on_conection_button_clicked()));

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
}


void MainWindow::updateWsRunnerPropertiesUI(wsr::ws_runner_properties wsRunnerProperties){

}

void MainWindow::updateConnectionMetainfoUI(wsr::connection_metainfo connectionMetainfo){
    this->connectionMetainfo = reformat(connectionMetainfo);

    ui->id_val->setText(this->connectionMetainfo.ID);
    ui->mms_val->setText(this->connectionMetainfo.maxTransferringSize);
    ui->uri_val->setText(this->connectionMetainfo.uri);
    ui->status_val->setText(this->connectionMetainfo.status);
}

void MainWindow::updateCommandsManagerPropertiesUI(cm::commands_manager_properties commandsManagerProperties){

}

void MainWindow::updateModulesManagerPropertiesUI(mm::modules_manager_properties modulesManagerProperties){

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


