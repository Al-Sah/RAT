//
// Created by al_sah on 12.05.21.
//

#ifndef CORE_MM_RESOURCES_H
#define CORE_MM_RESOURCES_H

#include "../CoreResources.h"
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <thread>

#include <utility>
#include <dlfcn.h>
#include <iostream>


namespace mm{

    struct ModulesManagerProperties{
        TargetsIds targets;
    };

}



#endif //CORE_MM_RESOURCES_H
