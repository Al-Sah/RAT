#ifndef TESTMAINWINDOW_H
#define TESTMAINWINDOW_H

#include "PicturesTakerDialogue.h"
#include <QMainWindow>
#include <QGroupBox>

namespace Ui {
class TestMainWindow;
}

class TestMainWindow : public QMainWindow{
    Q_OBJECT

public:
    PicturesTaker *test;
    explicit TestMainWindow(QWidget *parent = nullptr);
    ~TestMainWindow();

public slots:
    void addTarget();
    void reciveMessage();


private:
    Ui::TestMainWindow *ui;
};

#endif // TESTMAINWINDOW_H
