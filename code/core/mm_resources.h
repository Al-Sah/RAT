//
// Created by al_sah on 12.05.21.
//

#ifndef CORE_MM_RESOURCES_H
#define CORE_MM_RESOURCES_H

#include "configuration.h"
#include "models/TaskResult.h"
#include "models/payload_type.h"

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <utility>
#include <dlfcn.h>
#include <thread>


namespace mm{

    struct modules_manager_properties{
        Targets targets;
    };

}



#endif //CORE_MM_RESOURCES_H
