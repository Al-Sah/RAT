//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_APPLICATION_CONTEXT_H
#define BASIC_MODULE_APPLICATION_CONTEXT_H


#include "resources.h"

class ApplicationContext {

private:
    WSRunnerProperties wsRunnerProperties;
public:
    [[nodiscard]] const WSRunnerProperties &getWsRunnerProperties() const;

public:
    ApplicationContext();
};


#endif //BASIC_MODULE_APPLICATION_CONTEXT_H
