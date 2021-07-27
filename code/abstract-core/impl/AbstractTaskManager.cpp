//
// Created by al_sah on 09.03.21.
//


#include <utility>
#include "../TaskManaging/AbstractTaskManager.h"

std::string cmBool2str(bool b){
    return b ? "1" : "0";
}

std::string AbstractTaskManager::generate_section(std::string key, std::string value) const{
    return properties.delimiters.section + key + properties.delimiters.value + value;
}

void AbstractTaskManager::length_check(std::string &raw_envelope) {
    size_t size = raw_envelope.length() <= 97 ? raw_envelope.length()+2 : raw_envelope.length()+3;
    raw_envelope = std::to_string(size) + raw_envelope;
}

void AbstractTaskManager::handleRequestMessage(const std::string& src, PayloadType pt) {

    std::string errors;

    ParsedTextMessage* message = parseMessage(src,errors);
    errors.append(validate_parsed_message(*message));
    if(!errors.empty()){
        // TODO send error back
        return;
    }

    std::string pack_type = message->getPackageType();
    std::string request_id = message->getRequestId();

    // extracting payload and registering the task
    if(pack_type == properties.packages.firstPart){
        inboxTextMessagesBuffer.emplace(request_id, message->getPayload());
        if(message->getResponseType().empty() || message->getResponseType() != "none"){
            tasks.push_back(*(new TaskInfo(message)));
        }
    } else if (pack_type == properties.packages.continuation){
        inboxTextMessagesBuffer.find(request_id)->second.append( message->getPayload());

    }  else if (pack_type == properties.packages.lastPart){
        inboxTextMessagesBuffer.find(request_id)->second.append( message->getPayload());
        std::string tmp = message->getModule();
        taskExecutor->handleTask(tmp, request_id, inboxTextMessagesBuffer.find(request_id)->second, pt);
        inboxTextMessagesBuffer.erase(request_id);
    }

    if(pack_type == properties.packages.singleMessage){
        if(message->getResponseType().empty() || message->getResponseType() != "none"){
            tasks.push_back(*(new TaskInfo(message)));
        }
        std::string tmp = message->getModule();
        taskExecutor->handleTask(tmp, request_id, const_cast<std::string &>(message->getPayload()), pt);
    }
}

ParsedTextMessage* AbstractTaskManager::parseMessage(const std::string &src, std::string& errors) {

    std::string key, temp, envelope, payload;
    uint envelope_size;

    try{ // Extracting envelope
        std::string::size_type position = src.find_first_of(properties.delimiters.section);
        if(position == std::string::npos){
            errors.append("Failed to extract envelope");
            return new ParsedTextMessage();
        }
        envelope_size = std::stoi(src.substr(0, position));
        envelope = src.substr(position + 1, envelope_size - position -1);
    } catch (...){
        errors.append("Failed to extract envelope");
        return new ParsedTextMessage();
    }
    auto *parsedTextMessage = new ParsedTextMessage(src.substr(envelope_size));
    // Parsing envelope
    for(char ch : envelope){
        if(ch == properties.delimiters.value){
            key = temp;
            temp.clear();
        } else if(ch == properties.delimiters.section){
            errors.append(keyCheck(parsedTextMessage,key,temp));
            temp.clear();
        } else{
            temp += ch;
        }
    }
    if(!temp.empty()){
        errors.append(keyCheck(parsedTextMessage,key,temp));
    }
    return parsedTextMessage;
}

std::string AbstractTaskManager::validate_parsed_message(ParsedTextMessage& message) {
    std::string errors;
    if(message.getPackageType().empty() ||
        message.getModule().empty() ||
        message.getRequestId().empty()){
        errors.append("Empty parameters");
    }
    if(message.getTargetType() == properties.targets.control && message.getResponseType().empty()){
        errors.append("Empty parameter ResponseType");
    }
    return errors;
}

std::string AbstractTaskManager::keyCheck(ParsedTextMessage *message, std::string &key, std::string &value) const {
    if(key == properties.keys.targetModule){
        message->setModule(value);
    } else if(key == properties.keys.packageType){
        message->setPackageType(value);
    } else if(key == properties.keys.requestId){
        message->setRequestId(value);
    } else if(key == properties.keys.responseType){
        message->setResponseType(value);
    } else if(key == properties.keys.isLast){
        message->setIsLast(value);
    }else if(key == properties.keys.fullPayloadSize){
        message->setFullPayloadSize(value);
    }

    else if(key == properties.keys.targetType){
        message->setTargetType(value);
    }
    else if(key == properties.keys.targetId){
        message->setTargetId(value);
    } else{
        return "undefined key [" + key + "] with value [value]\n";
    }
    return "";
}

AbstractTaskManager::AbstractTaskManager(std::string &moduleId, std::string &version, cm::CommandsManagerProperties& properties)
                                         : Module(moduleId, version),properties(properties) {
    this->inboxMessagesHandler = std::thread(&AbstractTaskManager::runInboxMessagesHandler, this);
    this->resultMessagesHandler = std::thread(&AbstractTaskManager::runResultMessagesHandler, this);
}

//pthread_setname_np(pthread_self(),"command manager");
void AbstractTaskManager::runInboxMessagesHandler() {
    std::pair<std::string, PayloadType> inboxMessage;
    while (run){
        inboxMessagesMutex.lock();
        if( !inboxMessages.empty()){
            inboxMessage = inboxMessages.front();
            inboxMessages.pop();
            inboxMessagesMutex.unlock();
            handleRequestMessage(inboxMessage.first, inboxMessage.second);
            std::cout << "handleRequestMessage(message)\n";
        } else{
            inboxMessagesMutex.unlock();
            usleep(100);
        }
    }
}

void AbstractTaskManager::runResultMessagesHandler() {
    std::pair<TaskResult, ParsedTextMessage> result;
    while (run){
        resultMessagesMutex.lock();
        if( ! resultMessages.empty()){
            result = resultMessages.front();
            resultMessages.pop();
            resultMessagesMutex.unlock();
            handleTaskResult(result.first, result.second);
            std::cout << "handleRequestMessage(message)\n";
        } else{
            resultMessagesMutex.unlock();
            usleep(100);
        }
    }
}

void AbstractTaskManager::stop_work() {
    this->run = false;
}

void AbstractTaskManager::registerInboxTask(std::string &payload, PayloadType pt) {
    inboxMessagesMutex.lock();
    this->inboxMessages.emplace(payload, pt);
    inboxMessagesMutex.unlock();
}

void AbstractTaskManager::registerTaskResult(TaskResult &task, ParsedTextMessage &parsedMessage) {
    resultMessagesMutex.lock();
    this->resultMessages.emplace(std::make_pair(task,parsedMessage));
    resultMessagesMutex.unlock();
}



AbstractTaskManager::~AbstractTaskManager() {
    this->run = false;
    this->resultMessagesHandler.join();
    this->inboxMessagesHandler.join();
}


void AbstractTaskManager::setMessagesSender(WebSocketMessageSender *messagesSender) {
    AbstractTaskManager::messagesSender = messagesSender;
}

void AbstractTaskManager::setTaskExecutor(TaskExecutingDelegator *taskExecutor) {
    AbstractTaskManager::taskExecutor = taskExecutor;
}

