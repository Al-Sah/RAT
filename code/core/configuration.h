#ifndef BASIC_CONFIGURATION
#define BASIC_CONFIGURATION

//#define lambdas
//#define headers_includes

//#define BOT
//#define CONTROL
#include <string>
#include "cmake_configuration.h"

#ifndef BASIC_CONFIGURATION_Targets
#define BASIC_CONFIGURATION_Targets
struct Targets{
    std::string bot = "bot";
    std::string server = "server";
    std::string control = "control";

};
#endif

#endif //BASIC_CONFIGURATION


