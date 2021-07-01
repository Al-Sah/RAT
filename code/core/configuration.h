#ifndef BASIC_CONFIGURATION
#define BASIC_CONFIGURATION

//#define lambdas
//#define headers_includes

#include <Module.h>
#include <string>
#include "cmake_configuration.h"

#ifdef CONTROL_ENABLE
    #define TARGET "control"
#else
    #define TARGET "bot"
#endif

#ifndef BASIC_CONFIGURATION_Targets
#define BASIC_CONFIGURATION_Targets
struct Targets{
    std::string bot = "bot";
    std::string server = "server";
    std::string control = "control";
};
#endif

#endif //BASIC_CONFIGURATION