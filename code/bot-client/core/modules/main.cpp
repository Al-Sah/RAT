//
// Created by al_sah on 01.04.21.
//


#include "Module.h"
#include "echo/EchoModule.h"

//extern "C"
Module * getInstance(){
    return new MODULE_CLASS();
}

