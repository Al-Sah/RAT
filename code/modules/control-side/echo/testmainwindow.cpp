#include "testmainwindow.h"
#include "ui_testmainwindow.h"
#include "Echo.h"

TestMainWindow::TestMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::TestMainWindow){

    std::function<void(payload_type, void*, void*)> callback = [](payload_type, void*, void*){};

    this->test = new Echo(callback, this);

    ui->setupUi(this);
    test->getUI("zzzzzzz");
    test->getUI("zFDSzz")->show();

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(addTarget()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(reciveMessage()));

}

TestMainWindow::~TestMainWindow(){
    delete ui;
}


void TestMainWindow::addTarget(){
    QString res = "target";
    res.append(QString::number(qrand()%200 + 1));
    test->getUI(res)->show();
}

void TestMainWindow::reciveMessage(){
    QString res;
    if((qrand()%200 - 100) < 0){
         res = "zFDSzz target echo message";
    }
    if((qrand()%200 - 100) < 0){
         res = "zzzzzzz target echo message";
    }

    this->test->executeTask("", res.toStdString(), payload_type::text, nullptr);
}
