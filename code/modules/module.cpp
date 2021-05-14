//
// Created by al_sah on 01.04.21.
//

// just some magic... it works...
#define STRINGIFY_(val) #val
#define STRINGIFY(val) STRINGIFY_(val)
#include STRINGIFY(MODULE_FOLDER/MODULE_CLASS.h)

Module * getInstance(){
    return new MODULE_CLASS();
}