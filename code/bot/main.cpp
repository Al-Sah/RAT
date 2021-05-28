#include "../core/System.h"


int main() {
    System sys;

    sys.getWebsocketRunner()->setup_connection("ws://localhost:8080/control", "");

    while(true){};
    sys.getCommandsManager()->stop_work();
    sys.getWebsocketRunner()->close_connection();
    return 0;
}