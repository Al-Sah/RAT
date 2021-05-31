#include <iostream>

#include "testmainwindow.h"
#include "ServerInteraction.h"
#include <QApplication>


int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;

    QApplication app(argc, argv);
    TestMainWindow w;
    w.show();
    return app.exec();

    //test.executeTask("60f80df8-b0c6-4ff2-b3c4-3b40f86d5754 [test1 test2 test3][zzz zzz zz]", payload_type::text, nullptr);
    //return 0;
}
