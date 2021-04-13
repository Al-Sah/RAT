//
// Created by al_sah on 01.04.21.
//

#ifndef BASIC_MODULE_BOTMODULE_H
#define BASIC_MODULE_BOTMODULE_H
#include <string>

class BotModule {
public:
    virtual std::string name() = 0;
};

//typedef BotModule* (*getInstance_t)();
using getInstance_t = BotModule* (*)();

#endif //BASIC_MODULE_BOTMODULE_H
