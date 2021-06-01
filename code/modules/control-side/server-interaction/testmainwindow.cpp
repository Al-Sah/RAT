#include "testmainwindow.h"
#include "ui_testmainwindow.h"
#include "serverinteractiongroupbox.h"

TestMainWindow::TestMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::TestMainWindow){

    std::function<void(payload_type, void*, void*)> callback = [](payload_type, void*, void*){};

    this->test = new ServerInteraction(callback, this);

    ui->setupUi(this);
    ui->gridLayout->addWidget(test->getUI("zzzz"));

    test->getUI("zFDSzz")->show();

}

TestMainWindow::~TestMainWindow()
{
    delete ui;
}
