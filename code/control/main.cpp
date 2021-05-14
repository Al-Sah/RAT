#include "main_window.h"
#include "../client-core/System.h"

#include <QApplication>

int main(int argc, char *argv[]){

    System sys;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
