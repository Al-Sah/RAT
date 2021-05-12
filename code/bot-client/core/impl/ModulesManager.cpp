//
// Created by al_sah on 09.03.21.
//

#include "../ModulesManager.h"
#include "../modules/Module.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <utility>
#include <dlfcn.h>

#ifdef headers_includes
#include "../CommandsManager.h"

void ModulesManager::setCommandsManager(const std::weak_ptr<CommandsManager> &commandsManager) {
    ModulesManager::commandsManager = commandsManager;
}
#else
void ModulesManager::set_result_handler(std::function<void(TaskResult)> &result_handler) {
    this->result_handler = result_handler;
}
#endif


void ModulesManager::handleTask(std::string &module, std::string & task_id, std::shared_ptr<std::string> & payload_p) {
    std::cout<< "Executing task\n Module ["<< module <<"] " << " Task_id ["<< task_id <<"] Payload ["<< payload_p.operator*() <<"] !!!" << std::flush;
    std::string payload = *payload_p;

    Module* module_ptr = findModule(module);
    if(module_ptr == nullptr){
        std::string error = "target_module [" + module + "] not found";
        //this->handleResult(request_id, error);
        return;
    }

    std::function<void(payload_type, void*, bool)> callback = [this, task_id](payload_type result, void* result_payload, bool isLast){
        this->handleResult(result, result_payload, task_id, isLast);
    };

    std::thread thread(&Module::executeTask, module_ptr, payload, payload_type::text, callback);
    thread.detach();
}

void ModulesManager::handleResult(payload_type result, void *result_payload, std::string task_id, bool isLast) {
    std::string payload;
    if(result == payload_type::text){
        payload = *(std::string*) result_payload;
    }
    TaskResult message(task_id, payload, false, isLast);

#ifdef headers_includes
    commandsManager.lock()->handleResponseMessage(message);
#else
    this->result_handler(message);
#endif
}




/*void ModulesManager::handleResult(std::string &request_id, std::string &payload, bool isLast) {
    TaskResult message(request_id, payload, isLast);
#ifdef headers_includes
    commandsManager.lock()->handleResponseMessage(message);
#else
    this->result_handler(message);
#endif
}*/



ModulesManager::ModulesManager(const ModulesManagerProperties &properties) : properties(properties) {
    this->module_id = "ModulesManager";
    this->modules.push_back(this);
    this->loadExternalModules();
}




void ModulesManager::loadExternalModules(){

    boost::filesystem::path targetDir("./");
    boost::filesystem::recursive_directory_iterator iter(targetDir), eod;

    BOOST_FOREACH(boost::filesystem::path const& i, std::make_pair(iter, eod)){
        std::cout << " === : " << i.string() << " - " << i.extension() <<  std::endl;
        if (!is_regular_file(i)  ||   i.extension() != ".so"){
            continue;
        }

#ifdef WIN32
#else
        void *handle = dlopen (i.string().c_str(), RTLD_LAZY);
        auto pluggableModule = (getInstance_t)dlsym(handle, "_Z11getInstancev");
        //getInstance_t pluggableModule = (getInstance_t)dlsym(handle, "getInstance");
#endif
        Module *mod = pluggableModule();
        std::cout << "i'm  " << mod->getId() << std::endl;
        modules.push_back(mod);
    }
}

Module* ModulesManager::findModule(std::string& id) {

    for(auto module_ptr : modules){
        std::cout << module_ptr->getId();
        if(module_ptr->getId() == id){
            return module_ptr;
        }
    }
    return nullptr;
}

void ModulesManager::executeTask(std::string payload, payload_type pt, std::function<void(payload_type, void *, bool)> callback) {

}

void ModulesManager::registerModule(Module* module) {
    this->modules.push_back(module);
}

