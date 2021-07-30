//
// Created by al_sah on 18.07.21.
//

#ifndef BOT_APPLICATION_CONTEXT
#define BOT_APPLICATION_CONTEXT


#include <AppContext/AbstractApplicationContext.h>
#include "DataTransfer.h"

class ApplicationContext: public AbstractApplicationContext{

private:
    volatile bool run;
    std::thread notificator;
    DataTransfer* dataTransfer;
    void send_data();

public:
    ApplicationContext(std::string moduleId, std::string version, QWidget *parent);

    virtual ~ApplicationContext();
};


#endif //BOT_APPLICATION_CONTEXT
