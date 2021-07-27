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

    botResult request_info = *(botResult*)info;
    parsedMessage.setRequestId(request_info.task_id);
    parsedMessage.setIsLast(bool2str(request_info.isLast));
    TaskResult message(request_info.task_id, payload, result, request_info.isLast);

    taskResultRegister->registerTaskResult(message, parsedMessage);
}

void ModulesManager::registerModule(getModuleInstance func) {
    auto* module = (TaskExecutor*)func(defaultCallback, nullptr);
    std::string temp = module->getModuleId();
    this->modules.emplace(temp, module);
    //this->modules.insert(std::make_pair(module->getModuleId(), module));
    std::cout << " zzzzzzzzzzzzzzzzzzz";
}
void ModulesManager::registerModule(TaskExecutor* newModule) {
    this->modules.emplace(newModule->getModuleId(), newModule);
}

void ModulesManager::handleTask(std::string &module, std::string & task_id, std::string& payload, PayloadType pt) {

    TaskExecutor* module_ptr = findModule(module);
    if(module_ptr == nullptr){
        std::string error = "targetModule [" + module + "] not found";
        //this->handleModuleAction(requestId, error); //TODO
        return;
    }

    std::function<void(PayloadType, void*, bool)> callback = [this, task_id](PayloadType result, void* result_payload, bool isLast){
            auto *info = new botResult;
            info->task_id = task_id;
            info->isLast = isLast;
        this->handleModuleAction(result, result_payload, info);
    };
    std::thread thread(&TaskExecutor::executeTask, module_ptr, task_id, payload, pt, callback);
    thread.detach();

}

ModulesManager::ModulesManager(std::string moduleId, std::string version, mm::ModulesManagerProperties &properties)
        : AbstractModulesManager(moduleId, version, properties) {
}

TaskExecutor *ModulesManager::findModule(std::string &id) {
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

std::string ModulesManager::getModules() {
    std::string res = "botModules ";
    for(const auto& module: modules){
        res = res.append(module.first);
        res = res.append(" ");
    }
    return res;
}
