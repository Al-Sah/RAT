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
void ModulesManager::set_result_handler(std::function<void(TaskResult,ParsedTextMessage)> &result_handler) {
    this->request_handler = result_handler;
}
#define HANDLE_MODULE_RESPONSE(task, parsedMessage)(this->request_handler(task, parsedMessage))
#endif
#define GET_INSTANCE_FUNCTION "getInstance"
//"_Z11getInstancev"

std::string bool2str(bool b){
    return b ? "1" : "0";
}

void ModulesManager::handleTask(std::string &module, std::string & task_id, std::string& payload, payload_type pt) {
    //std::cout<< "Executing task Module ["<< module <<"] " << " Task_id ["<< task_id <<"] Payload ["<< payload_p.operator*() <<"] !!!" << std::flush;

    Module* module_ptr = _findModule(module);
    if(module_ptr == nullptr){
        std::string error = "target_module [" + module + "] not found";
        //this->handleModuleAction(request_id, error); //TODO
        return;
    }

#ifdef BOT_ENABLE
    std::function<void(payload_type, void*, bool)> callback = [this, task_id](payload_type result, void* result_payload, bool isLast){
            botResult *info = new botResult;
            info->task_id = task_id;
            info->isLast = isLast;
        this->handleModuleAction(result, result_payload, info);
    };
    std::thread thread(&Module::executeTask, module_ptr, task_id, payload, pt, callback);
    thread.detach();
#else
    std::thread thread(&Module::executeTask, module_ptr, task_id, payload, pt, nullptr);
    thread.detach();
#endif
}

void ModulesManager::handleModuleAction(payload_type result, void *result_payload, void* info) {
    ParsedTextMessage parsedMessage;
    std::string payload;
    payload = *(std::string*) result_payload;


#ifdef BOT_ENABLE
    botResult request_info = *(botResult*)info;
    parsedMessage.setRequestId(request_info.task_id);
    parsedMessage.setIsLast(bool2str(request_info.isLast));
    TaskResult message(request_info.task_id, payload, result, request_info.isLast);
#endif
#ifdef CONTROL_ENABLE
    controlRequest request_info = *(controlRequest*)info;

    parsedMessage.setModule(request_info.target_module);

    switch (request_info.target_type) {
        case targets_enum::bot:
            parsedMessage.setTargetType(properties.targets.bot);
            break;
        case targets_enum::server:
            parsedMessage.setTargetType(properties.targets.server);
            break;
        case targets_enum::control:
            parsedMessage.setTargetType(properties.targets.control);
            break;
    }

    parsedMessage.setTargetId(request_info.target_id);
    parsedMessage.setResponseType(request_info.required_response);
    parsedMessage.setRequestId(request_info.task_id);

    TaskResult message(request_info.task_id, payload, result, true);
#endif

    HANDLE_MODULE_RESPONSE(message, parsedMessage);
}


ModulesManager::ModulesManager(const mm::modules_manager_properties &properties, void * ui) : properties(properties) {
    this->module_id = TARGET"ModulesManager";
    this->modules.emplace(module_id,this);

    this->default_modules_callback = [this](payload_type pt, void* payload, void* data){
        this->handleModuleAction(pt, payload, data);
    };
#ifdef CONTROL_ENABLE
    this->ui = ui;
    this->dataTransfer = new DataTransfer( (QObject*)ui);
#endif
    this->loadExternalModules();
}


void ModulesManager::loadExternalModules(){

    boost::filesystem::path targetDir("./");
    boost::filesystem::recursive_directory_iterator iter(targetDir), eod;

    BOOST_FOREACH(boost::filesystem::path const& i, std::make_pair(iter, eod)){
        if (!is_regular_file(i) || i.extension() != ".so"){
            continue;
        }
        std::cout << i;
#ifdef WIN32
#else
        void *handle = dlopen (i.string().c_str(), RTLD_LAZY);
        auto pluggableModule = (getInstance_t)dlsym(handle, GET_INSTANCE_FUNCTION);
#endif
        if(pluggableModule != nullptr){
            registerModule(pluggableModule);
        }
    }
}

Module* ModulesManager::_findModule(std::string& id) {
    if(modules.find(id) != modules.end()){
        return modules[id];
    }
    return nullptr;
    /*for(auto module_ptr : modules){
        std::cout << module_ptr->getId();
        if(module_ptr->getId() == id){
            return module_ptr;
        }
    }*/
}

Module *ModulesManager::findModule(std::string id) {
    if(modules.find(id) != modules.end()){
        return modules[id];
    }
    return nullptr;
    /*for(auto module_ptr : modules){
        std::cout << module_ptr->getId();
        if(module_ptr->getId() == id){
            return module_ptr;
        }
    }
    return nullptr;*/
}

void ModulesManager::executeTask(std::string task, std::string payload, payload_type pt, std::function<void(payload_type, void *, bool)> callback) {
    if(pt == payload_type::text && payload == "getModules"){
        std::string res = getModules();
        callback(pt, &res, true);
    }
}

void ModulesManager::registerModule(getInstance_t func) {

#ifdef BOT_ENABLE
    Module *module = func(default_modules_callback, nullptr);
    this->modules.emplace(module->getId(), module);
#endif

#ifdef CONTROL_ENABLE
    Module *module = func(default_modules_callback, ui);
    this->modules.emplace(module->getId(), module);
#endif

    // TODO if Mirror or just control, pass handleModuleAction
}

void ModulesManager::registerModule(Module *module) {
    this->modules.emplace(module->getId(), module);
}

void ModulesManager::handleTask(Module *module, payload_type pt, std::string payload) {
    std::thread thread(&Module::executeTask, module, "", payload, pt, nullptr);
    thread.detach();
}

std::string ModulesManager::getModules() {
    std::string res = "botModules ";
    for(auto module: modules){
        res = res.append(module.first);
        res = res.append(" ");
    }
    return res;
}



