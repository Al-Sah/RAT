//
// Created by al_sah on 09.03.21.
//

#include "../ModulesManager.h"


#ifdef headers_includes
#include "../CommandsManager.h"

void ModulesManager::setCommandsManager(const std::weak_ptr<CommandsManager> &commandsManager) {
    ModulesManager::commandsManager = commandsManager;
}
#define HANDLE_MODULE_RESPONSE(message)(commandsManager.lock()->handleResponseMessage(message))
#else
void ModulesManager::set_result_handler(std::function<void(TaskResult)> &result_handler) {
    this->result_handler = result_handler;
}
#define HANDLE_MODULE_RESPONSE(message)(this->result_handler(message))
#endif
#define GET_INSTANCE_FUNCTION "_Z11getInstancev"


void ModulesManager::handleTask(std::string &module, std::string & task_id, std::shared_ptr<std::string> & payload_p) {
    //std::cout<< "Executing task Module ["<< module <<"] " << " Task_id ["<< task_id <<"] Payload ["<< payload_p.operator*() <<"] !!!" << std::flush;
    std::string payload = *payload_p;

    Module* module_ptr = findModule(module);
    if(module_ptr == nullptr){
        std::string error = "target_module [" + module + "] not found";
        //this->handleResult(request_id, error); //TODO
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
    HANDLE_MODULE_RESPONSE(message);
}


ModulesManager::ModulesManager(const mm::modules_manager_properties &properties) : properties(properties) {
    this->module_id = "ModulesManager";
    this->modules.push_back(this);
    this->loadExternalModules();
}


void ModulesManager::loadExternalModules(){

    boost::filesystem::path targetDir("./");
    boost::filesystem::recursive_directory_iterator iter(targetDir), eod;

    BOOST_FOREACH(boost::filesystem::path const& i, std::make_pair(iter, eod)){
        if (!is_regular_file(i)  ||   i.extension() != ".so"){
            continue;
        }

#ifdef WIN32
#else
        void *handle = dlopen (i.string().c_str(), RTLD_LAZY);
        auto pluggableModule = (getInstance_t)dlsym(handle, GET_INSTANCE_FUNCTION);
#endif
        Module *mod = pluggableModule();
        std::cout << "Registered new external module [" << mod->getId() << "]\n";
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

