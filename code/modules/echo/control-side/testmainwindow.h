#ifndef TESTMAINWINDOW_H
#define TESTMAINWINDOW_H

#include "Echo.h"
#include <QMainWindow>
#include <QGroupBox>

namespace Ui {
class TestMainWindow;
}

class TestMainWindow : public QMainWindow{
    Q_OBJECT

public:
    Echo *test;
    explicit TestMainWindow(QWidget *parent = nullptr);
    ~TestMainWindow();

public slots:
    void addTarget();
    void reciveMessage();


private:
    Ui::TestMainWindow *ui;
};

#endif // TESTMAINWINDOW_H
