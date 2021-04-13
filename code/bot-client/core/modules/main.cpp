//
// Created by al_sah on 01.04.21.
//


#include "BotModule.h"
#include "echo/EchoModule.h"

//extern "C"
BotModule * getInstance(){
    return new MODULE_CLASS();
}

