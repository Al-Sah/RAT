#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../core/System.h"
#include "info_blocks.h"
#include "ConnectionSetupDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    System* system;
    ConnectionSetupDialog* csd;

    ConnectionMetaInfo connectionMetainfo;

    void setup_connection(QString url, QString pawd);

    ConnectionMetaInfo reformat(wsr::connection_metainfo connectionMetainfo);

public slots:
    void on_conection_button_clicked();

    void updateWsRunnerPropertiesUI(wsr::ws_runner_properties wsRunnerProperties);
    void updateConnectionMetainfoUI(wsr::connection_metainfo connectionMetainfo);
    void updateCommandsManagerPropertiesUI(cm::commands_manager_properties commandsManagerProperties);
    void updateModulesManagerPropertiesUI(mm::modules_manager_properties modulesManagerProperties);


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();




};
#endif // MAINWINDOW_H
