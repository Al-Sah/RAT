//
// Created by al_sah on 09.03.21.
//

#include "../CommandsManager.h"
#include <utility>

#ifdef headers_includes
#include "../WebsocketRunner.h"
#endif



std::string bool2str(bool b){
    return b ? "1" : "0";
}

std::string CommandsManager::generate_section(std::string key, std::string value) const{
    return properties.delimiters.section + key + properties.delimiters.value + value;
}

void CommandsManager::length_check(std::string &raw_envelope) {
    size_t size = raw_envelope.length() <= 97 ? raw_envelope.length()+2 : raw_envelope.length()+3;
    raw_envelope = std::to_string(size) + raw_envelope;
}

void CommandsManager::handleRequestMessage(const std::string& src) {

    ParsedTextMessage* message = parseMessage(src);
    std::string pack_type = message->getPackageType();
    std::string request_id = message->getRequestId();

    // extracting payload and registering the task
    if(pack_type == "first"){
        inboxTextMessagesBuffer.emplace(request_id, std::make_shared<std::string>(message->getPayload()));
        if(message->getResponseType() != "none"){
            tasks.push_back(*(new Task(message)));
        }
    } else if (pack_type == "con"){
        inboxTextMessagesBuffer.find(request_id)->second->append( message->getPayload());
    }  else if (pack_type == "last"){


        inboxTextMessagesBuffer.find(request_id)->second->append( message->getPayload());
#ifdef headers_includes
        modulesManager.lock()->executeTask(
                        message->getModule(),
                        request_id,
                        (inboxTextMessagesBuffer.find(request_id))->second);
#else
        task_executor(message->getModule(),request_id, std::make_shared<std::string>(message->getPayload()));
#endif
        inboxTextMessagesBuffer.erase(request_id);
    }

    if(pack_type == "single"){
        if(message->getResponseType() != "none"){
            tasks.push_back(*(new Task(message)));
        }
#ifdef headers_includes
        modulesManager.lock()->executeTask(
                        message->getModule(),
                        request_id,
                        (inboxTextMessagesBuffer.find(request_id))->second);
#else
        task_executor(message->getModule(),request_id, std::make_shared<std::string>(message->getPayload()));
#endif
    }

}

ParsedTextMessage* CommandsManager::parseMessage(const std::string &src) {

    std::string key, temp, envelope, payload;
    std::map<std::string, std::string> envelopeParams;

    std::string::size_type position = src.find_first_of(properties.delimiters.section);
    uint envelope_size = std::stoi(src.substr(0, position));
    envelope = src.substr(position + 1, envelope_size - position -1 );
    auto *parsedTextMessage = new ParsedTextMessage(src.substr(envelope_size));

    if(properties.enable_args_id){
        for(char ch : envelope){
            if(ch == properties.delimiters.value){
                key = temp;
                temp.clear();
            } else if(ch == properties.delimiters.section){
                keyCheck(parsedTextMessage,key,temp);
                envelopeParams.emplace(key,temp);
                temp.clear();
            } else{
                temp += ch;
            }
        }
        if(!temp.empty()){
            keyCheck(parsedTextMessage,key,temp);
        }
    } else{
/*        position = 0;
        while (position != std::string::npos){
            position = envelope.find_first_of('#', position);
            std::string::npos
        }*/
    }
    parsedTextMessage->setEnvelopeParams(envelopeParams);
    return parsedTextMessage;

    //return new ParsedTextMessage(envelopeParams, src.substr(envelope_size));
}

bool CommandsManager::validate_parsed_message(ParsedTextMessage message) {


    return false;
}

void CommandsManager::keyCheck(ParsedTextMessage *message, std::string &key, std::string &src) const {
    if(key == properties.short_args_id.module || key == properties.full_args_id.module){
        message->setModule(src);
    } else if(key == properties.short_args_id.package_type || key == properties.full_args_id.package_type){
        message->setPackageType(src);
    } else if(key == properties.short_args_id.request_id || key == properties.full_args_id.request_id){
        message->setRequestId(src);
    } else if(key == properties.short_args_id.response_type || key == properties.full_args_id.response_type){
        message->setResponseType(src);
    }
}

void CommandsManager::handleResponseMessage(std::string &task_id, std::shared_ptr<std::string> payload, bool isLast) {
    // TODO get max transferring message size, check it parse message by parts

    std::string result = generate_section(properties.short_args_id.package_type, "single") +
                         generate_section(properties.short_args_id.request_id, task_id) +
                         generate_section(properties.short_args_id.is_last, bool2str(isLast));

    this->length_check(result);
    result.append(payload.operator*());


#ifdef headers_includes
    websocketRunner.lock()->send_message(result);
#else
   this->send_message_function(result);
#endif
}



CommandsManager::CommandsManager(CommandsManagerProperties properties) : properties(std::move(properties)) {
    this->ws_listener = std::thread(&CommandsManager::run_ws_listener, this);

}

[[noreturn]] void CommandsManager::run_ws_listener() {
    std::string message;
    //pthread_setname_np(pthread_self(),"command manager");
    while (true){
        if(this->inboxMessages.empty()){
            std::cout << "zzzzzzz\n";
            sleep(5);
        } else {
            message = inboxMessages.front();
            inboxMessages.pop();
            handleRequestMessage(message);
            std::cout << "handleRequestMessage(message)\n";
        }
    }

}


void CommandsManager::add_new_message(std::string &payload) {
    this->inboxMessages.emplace(payload);
}


#ifdef headers_includes
void CommandsManager::setModulesManager(const std::weak_ptr<ModulesManager> &modulesManager) {
    CommandsManager::modulesManager = modulesManager;
}

void CommandsManager::setWebsocketRunner(const std::weak_ptr<WebsocketRunner> &websocketRunner) {
    CommandsManager::websocketRunner = websocketRunner;
}
#else
void CommandsManager::setMessageSender(std::function<bool(std::string)> &sender) {
    this->send_message_function = sender;
}
void CommandsManager::setTaskExecutor(std::function<void(std::string, std::string, std::shared_ptr<std::string>)> &task_executor) {
    this->task_executor = task_executor;
}
#endif
