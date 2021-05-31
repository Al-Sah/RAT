#ifndef TESTMAINWINDOW_H
#define TESTMAINWINDOW_H

#include "ServerInteraction.h"
#include <QMainWindow>
#include <QGroupBox>

namespace Ui {
class TestMainWindow;
}

class TestMainWindow : public QMainWindow{
    Q_OBJECT

public:
    ServerInteraction *test;
    explicit TestMainWindow(QWidget *parent = nullptr);
    ~TestMainWindow();


private:
    Ui::TestMainWindow *ui;
};

#endif // TESTMAINWINDOW_H
