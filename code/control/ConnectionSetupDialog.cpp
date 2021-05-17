#include "ConnectionSetupDialog.h"
#include "ui_ConnectionSetupDialog.h"

ConnectionSetupDialog::ConnectionSetupDialog(QWidget *parent) : QDialog(parent),ui(new Ui::connectionSetupDialog){
    ui->setupUi(this);


    connect(ui->cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->ok, SIGNAL(clicked(bool)), this, SLOT(saveAndQuit(link, pswd)));
}

ConnectionSetupDialog::~ConnectionSetupDialog(){


    delete ui;
}


void ConnectionSetupDialog::saveAndQuit(QString &link,QString &pswd){

    link = ui->link->text();
    pswd = ui->password->text();
    this->hide();
}
