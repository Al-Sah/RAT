//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_MODULESMANAGER_H
#define BASIC_MODULE_MODULESMANAGER_H


#include <string>
#include <list>
#include <map>
#include <memory>

class ModulesManager {


public:
    // temp
    void executeTask(std::string module, std::string task_id, std::shared_ptr<std::string> payload);
    //void on_module_message();




};


#endif //BASIC_MODULE_MODULESMANAGER_H
