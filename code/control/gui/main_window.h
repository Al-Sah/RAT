#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QString>
#include <core-impl/Control.h>
#include "info_blocks.h"
#include "ConnectionSetupDialog.h"
#include "UIModule.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    Control* system;
    ConnectionSetupDialog* csd;
    QDateTime lastConnentionTime;

    ConnectionMetaInfo connectionMetainfo;
    WSRProperties wsrProperties;


    UIModule* serverInteraction;

    void setup_connection(QString url, QString pawd);
    ConnectionMetaInfo reformat(wsr::ConnectionMetainfo connectionMetainfo);
    WSRProperties reformat(wsr::WebSocketRunnerProperties properties);

public slots:
    void on_connection_button_clicked();

    void updateWsRunnerPropertiesUI(wsr::WebSocketRunnerProperties wsRunnerProperties);
    void updateConnectionMetainfoUI(wsr::ConnectionMetainfo connectionMetainfo);
    void updateCommandsManagerPropertiesUI(cm::CommandsManagerProperties commandsManagerProperties);
    void updateModulesManagerPropertiesUI(mm::ModulesManagerProperties modulesManagerProperties);

    void showModule(QString moduleID, QString targetID);


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();




};
#endif // MAINWINDOW_H
