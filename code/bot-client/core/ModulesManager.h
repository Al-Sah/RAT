//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_MODULESMANAGER_H
#define BASIC_MODULE_MODULESMANAGER_H


#include <string>
#include <list>
#include <map>

class ModulesManager {

private:
    void executeTask(std::string id);

    // map < request_id, pair <module, response >
    std::map<std::string, std::pair<std::string, std::string>> tasks;
public:

    void register_task();
};


#endif //BASIC_MODULE_MODULESMANAGER_H
