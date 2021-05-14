#include "../client-core/System.h"


int main() {
    System sys;

    sys.getWebsocketRunner()->setup_connection("ws://localhost:8080/bot");

        sleep(1);

    sys.getCommandsManager()->stop_work();
    sys.getWebsocketRunner()->close_connection();
    return 0;
}