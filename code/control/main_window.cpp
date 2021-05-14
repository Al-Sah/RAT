#include "main_window.h"
#include "./ui_main_window.h"
#include <QPushButton>
#include "test.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
   ui->setupUi(this);

   getSomeLayout(ui->gridLayout, this);

  // ui->gridLayout->addWidget(new QPushButton("OK"), 0, 0);
   //ui->gridLayout->addWidget(new QPushButton("OK", this), 1, 1);
   //ui->gridLayout->addWidget(new QPushButton("OK", this), 2, 2);

}

MainWindow::~MainWindow(){
    delete ui;
}

