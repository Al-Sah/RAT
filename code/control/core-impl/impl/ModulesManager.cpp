//
// Created by al_sah on 17.07.21.
//

#include "../ModulesManager.h"
#define GET_INSTANCE_FUNCTION "getInstance"

inline std::string bool2str(bool b){
    return b ? "1" : "0";
}

void ModulesManager::handleModuleAction(PayloadType result, void *result_payload, void *info) {

    ParsedTextMessage parsedMessage;
    std::string payload;
    payload = *(std::string*) result_payload;

    controlRequest request_info = *(controlRequest*)info;

    parsedMessage.setModule(request_info.target_module);

    switch (request_info.target_type) {
        case Targets::bot:
            parsedMessage.setTargetType(properties.targets.bot);
            break;
        case Targets::server:
            parsedMessage.setTargetType(properties.targets.server);
            break;
        case Targets::control:
            parsedMessage.setTargetType(properties.targets.control);
            break;
    }

    parsedMessage.setTargetId(request_info.target_id);
    parsedMessage.setResponseType(request_info.required_response);
    parsedMessage.setRequestId(request_info.task_id);

    TaskResult message(request_info.task_id, payload, result, true);

    taskResultRegister->registerTaskResult(message, parsedMessage);
}

void ModulesManager::registerModule(getModuleInstance func) {
    auto* module = (UIModule*)func(defaultCallback, ui);
    std::string moduleId = module->getModuleId();
    this->modules.emplace(module->getModuleId(), module);
}

void ModulesManager::registerModule(UIModule* newModule) {
    this->modules.emplace(newModule->getModuleId(), newModule);
}

void ModulesManager::handleTask(std::string &module, std::string & task_id, std::string& payload, PayloadType pt) {

    UIModule* module_ptr = findModule(module);
    if(module_ptr == nullptr){
        std::string error = "targetModule [" + module + "] not found";
        //this->handleModuleAction(requestId, error); //TODO
        return;
    }
    std::thread thread(&UIModule::executeTask, module_ptr, task_id, payload, pt, nullptr);
    thread.detach();
}

UIModule *ModulesManager::findModule(std::string id) {
    if(modules.find(id) != modules.end()){
        return modules[id];
    }
    return nullptr;
}

void ModulesManager::loadExternalModules() {


    boost::filesystem::path targetDir("./");
    boost::filesystem::recursive_directory_iterator iter(targetDir), eod;

    BOOST_FOREACH(boost::filesystem::path const& i, std::make_pair(iter, eod)){
#ifdef WIN32
#else
        if (!is_regular_file(i) || i.extension() != ".so"){
            continue;
        }
        std::cout << i;
        void *handle = dlopen (i.string().c_str(), RTLD_LAZY);
        auto pluggableModule = (getModuleInstance) dlsym(handle, GET_INSTANCE_FUNCTION);
#endif
        if(pluggableModule != nullptr){
            registerModule(pluggableModule);
        }
    }
}

ModulesManager::ModulesManager(std::string moduleId, std::string version,
                               mm::ModulesManagerProperties &properties, QWidget *ui)
                               : AbstractModulesManager(moduleId, version, properties) {
    this->ui = ui;
    this->dataTransfer = new DataTransfer(ui);
}
