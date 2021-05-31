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

#include <sstream>
#include <random>

std::string generate_uuid_v4() {
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    };
    std::cout << "UUID:  " << ss.str() << std::endl;
    return ss.str();
}




void ModulesManager::handleTask(std::string &module, std::string & task_id, std::shared_ptr<std::string> & payload_p) {
    //std::cout<< "Executing task Module ["<< module <<"] " << " Task_id ["<< task_id <<"] Payload ["<< payload_p.operator*() <<"] !!!" << std::flush;
    std::string payload = *payload_p;

    Module* module_ptr = _findModule(module);
    if(module_ptr == nullptr){
        std::string error = "target_module [" + module + "] not found";
        //this->handleModuleAction(request_id, error); //TODO
        return;
    }

#ifdef BOT_ENABLE
    std::function<void(payload_type, void*, bool)> default_modules_callback = [this, task_id](payload_type result, void* result_payload, bool isLast){
            botResult *info = new botResult;
            info->task_id = task_id;
            info->isLast = isLast;
        this->handleModuleAction(result, result_payload, info);
    };
    std::thread thread(&Module::executeTask, module_ptr, payload, payload_type::text, default_modules_callback);
    thread.detach();
#else
    std::thread thread(&Module::executeTask, module_ptr, payload, payload_type::text, nullptr);
    thread.detach();
#endif
}

void ModulesManager::handleModuleAction(payload_type result, void *result_payload, void* info) {
    ParsedTextMessage parsedMessage;
    std::string payload;
    if(result == payload_type::text){
        payload = *(std::string*) result_payload;
    }

#ifdef BOT_ENABLE
    botResult request_info = *(botResult*)info;
    parsedMessage.setRequestId(request_info.task_id);
    parsedMessage.setIsLast(bool2str(request_info.isLast));
    TaskResult message(request_info.task_id, payload, result, request_info.isLast);
#endif
#ifdef CONTROL_ENABLE
    controlRequest request_info = *(controlRequest*)info;
    std::string uuid(generate_uuid_v4());
    std::cout << std::endl << uuid << std::endl;

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
    parsedMessage.setRequestId(uuid);

    TaskResult message(uuid, payload, result, true);
#endif

    HANDLE_MODULE_RESPONSE(message, parsedMessage);
}


ModulesManager::ModulesManager(const mm::modules_manager_properties &properties, void * ui) : properties(properties) {
    this->module_id = "ModulesManager";
    this->modules.push_back(this);

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
    for(auto module_ptr : modules){
        std::cout << module_ptr->getId();
        if(module_ptr->getId() == id){
            return module_ptr;
        }
    }
    return nullptr;
}

Module *ModulesManager::findModule(std::string id) {
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

void ModulesManager::registerModule(getInstance_t func) {

#ifdef BOT_ENABLE
    Module *module = func(default_modules_callback, nullptr);
    this->modules.push_back(module);
#endif

#ifdef CONTROL_ENABLE
    Module *module = func(default_modules_callback, ui);
    this->modules.push_back(module);
#endif

    // TODO if Mirror or just control, pass handleModuleAction
}

void ModulesManager::registerModule(Module *module) {
    this->modules.push_back(module);
}

void ModulesManager::handleTask(Module *module, payload_type pt, std::string payload) {
    std::thread thread(&Module::executeTask, module, payload, pt, nullptr);
    thread.detach();
}



