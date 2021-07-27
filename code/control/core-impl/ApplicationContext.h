//
// Created by al_sah on 18.07.21.
//

#ifndef BOT_APPLICATION_CONTEXT
#define BOT_APPLICATION_CONTEXT


#include <AppContext/AbstractApplicationContext.h>

class ApplicationContext: public AbstractApplicationContext{

public:
    ApplicationContext(std::string moduleId, std::string version);
};


#endif //BOT_APPLICATION_CONTEXT
