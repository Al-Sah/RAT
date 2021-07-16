//
// Created by al_sah on 09.03.21.
//


#include <utility>

#include "../CommandsManager.h"


#ifdef headers_includes
    #include "../WebsocketRunner.h"
    #define EXECUTE(module,task_id,payload) (modulesManager.lock()->executeTask(module,task_id,payload))
    #define SEND(message) (websocketRunner.lock()->send_message(message))

    void CommandsManager::setModulesManager(const std::weak_ptr<ModulesManager> &modulesManager) {
        CommandsManager::modulesManager = modulesManager;
    }
    void CommandsManager::setWebsocketRunner(const std::weak_ptr<WebsocketRunner> &websocketRunner) {
        CommandsManager::websocketRunner = websocketRunner;
    }
#else
    #define EXECUTE(module,task_id,payload,payload_type) (task_executor(module,task_id,payload,payload_type))
    #define SEND(message, type) (this->send_message_function(message, type))

    void CommandsManager::setMessageSender(std::function<bool(std::string, payload_type)> &sender) {
        this->send_message_function = sender;
    }
    void CommandsManager::setTaskExecutor(std::function<void(std::string, std::string, std::string, payload_type pt)> &task_executor) {
        this->task_executor = task_executor;
    }
#endif


std::string cmBool2str(bool b){
    return b ? "1" : "0";
}

std::string CommandsManager::generate_section(std::string key, std::string value) const{
    return properties.delimiters.section + key + properties.delimiters.value + value;
}

void CommandsManager::length_check(std::string &raw_envelope) {
    size_t size = raw_envelope.length() <= 97 ? raw_envelope.length()+2 : raw_envelope.length()+3;
    raw_envelope = std::to_string(size) + raw_envelope;
}

void CommandsManager::handleRequestMessage(const std::string& src, payload_type pt) {

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
    if(pack_type == properties.packages.first_part){
        inboxTextMessagesBuffer.emplace(request_id, std::make_shared<std::string>(message->getPayload()));
        if(message->getResponseType().empty() || message->getResponseType() != "none"){
            tasks.push_back(*(new TaskInfo(message)));
        }
    } else if (pack_type == properties.packages.continuation){
        inboxTextMessagesBuffer.find(request_id)->second->append( message->getPayload());

    }  else if (pack_type == properties.packages.last_part){
        inboxTextMessagesBuffer.find(request_id)->second->append( message->getPayload());
        EXECUTE(message->getModule(), request_id, *inboxTextMessagesBuffer.find(request_id)->second, pt);
        inboxTextMessagesBuffer.erase(request_id);
    }

    if(pack_type == properties.packages.single_message){
        if(message->getResponseType().empty() || message->getResponseType() != "none"){
            tasks.push_back(*(new TaskInfo(message)));
        }
        EXECUTE(message->getModule(), request_id, message->getPayload(),pt);
    }
}

ParsedTextMessage* CommandsManager::parseMessage(const std::string &src, std::string& errors) {

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

std::string CommandsManager::validate_parsed_message(ParsedTextMessage& message) {
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

std::string CommandsManager::keyCheck(ParsedTextMessage *message, std::string &key, std::string &value) const {
    if(key == properties.keys.target_module){
        message->setModule(value);
    } else if(key == properties.keys.package_type){
        message->setPackageType(value);
    } else if(key == properties.keys.request_id){
        message->setRequestId(value);
    } else if(key == properties.keys.response_type){
        message->setResponseType(value);
    } else if(key == properties.keys.is_last){
        message->setIsLast(value);
    }else if(key == properties.keys.full_payload_size){
        message->setFullPayloadSize(value);
    }

    else if(key == properties.keys.target_type){
        message->setTargetType(value);
    }
    else if(key == properties.keys.target_id){
        message->setTargetId(value);
    } else{
        return "undefined key [" + key + "] with value [value]\n";
    }
    return "";
}

void CommandsManager::handleResponseMessage(TaskResult &message, ParsedTextMessage &parsedTextMessage) {
    // TODO get max transferring message size, check it parse message by parts

    size_t max_message_size = get_max_message_size(), current_message_size = message.getPayload().size();
    size_t parts, message_part_size;


    if(current_message_size + 150 > max_message_size){
        long double temp = (long double)current_message_size / max_message_size;
        parts = temp - (int)temp > 0 ? (int)(temp + 1) : (int)temp;
        // add max headers size to result message

        message_part_size = current_message_size / parts; //

        temp = ((long double)current_message_size + (long double)parts*150) / max_message_size;
        parts = temp - (int)temp > 0 ? (int)(temp + 1) : (int)temp;

        message_part_size = (current_message_size - parts*150) / parts; //

        const std::string& taskId = message.getTaskId();

        std::list<std::string> result_message_parts;

        std::string payload_to_parse;

        if(message.getPayloadType() == payload_type::file_hath){
            std::ifstream file(message.getPayload());
            if (file.is_open()) {
                std::stringstream ss;
                ss << file.rdbuf();
                payload_to_parse = ss.str();
                file.close();
            } else{
                std::cout << "Failed to read file!";
            }
        } else {
            payload_to_parse = message.getPayload();
        }

        for(int i = 1; i <= parts; i++){
            std::string result;
            if( i == 1){
                result = generate_section(properties.keys.package_type, properties.packages.first_part)
#ifdef CONTROL_ENABLE
                        .append(generate_section(properties.keys.target_type, parsedTextMessage.getTargetType()))
                        .append(generate_section(properties.keys.target_id, parsedTextMessage.getTargetId()))
                        .append(generate_section(properties.keys.target_module, parsedTextMessage.getModule()))
                        .append(generate_section(properties.keys.response_type, parsedTextMessage.getResponseType()))
#endif
                        .append(generate_section(properties.keys.request_id, message.getTaskId()))
                        .append(generate_section(properties.keys.is_last, cmBool2str(message.getIsLast())))
                        .append(generate_section(properties.keys.full_payload_size, std::to_string(message.getPayload().size())));

                this->length_check(result);
                result += payload_to_parse.substr(0,message_part_size);
                std::cout << " Sending bytes from    0  to  " << message_part_size-1 << std::endl;
            } else if (i == parts){
                result = generate_section(properties.keys.package_type, properties.packages.last_part)
#ifdef CONTROL_ENABLE
                        .append(generate_section(properties.keys.target_type, parsedTextMessage.getTargetType()))
                        .append(generate_section(properties.keys.target_id, parsedTextMessage.getTargetId()))
#endif
                        .append(generate_section(properties.keys.request_id, message.getTaskId()))
                        .append(generate_section(properties.keys.is_last, cmBool2str(message.getIsLast())));;
                this->length_check(result);
                result += payload_to_parse.substr(message_part_size*(i-1));
                std::cout << " Sending bytes from    0  to  " << message_part_size*(i-1)  << "  " << payload_to_parse.substr(message_part_size*(i-1)).size() << std::endl;
            } else{
                result = generate_section(properties.keys.package_type, properties.packages.continuation)
#ifdef CONTROL_ENABLE
                        .append(generate_section(properties.keys.target_type, parsedTextMessage.getTargetType()))
                        .append(generate_section(properties.keys.target_id, parsedTextMessage.getTargetId()))
#endif
                        .append(generate_section(properties.keys.request_id, message.getTaskId()));

                this->length_check(result);
                result += payload_to_parse.substr(message_part_size*(i-1), message_part_size);
                std::cout << " AAAAA " << message_part_size*(i-1) << "    "   << message_part_size*i-1  << std::endl;
            }
            result_message_parts.push_back(result);
        }
        for (const auto& part: result_message_parts){
            std::cout << " \n  SENDING " <<  part.size();
            SEND(part, message.getPayloadType());
        }

    } else{
        std::string result;

#ifdef BOT_ENABLE
        result = generate_section(properties.keys.package_type, properties.packages.single_message) +
                 generate_section(properties.keys.request_id, message.getTaskId()) +
                 generate_section(properties.keys.is_last, cmBool2str(message.getIsLast()));
#endif

#ifdef CONTROL_ENABLE
        result = generate_section(properties.keys.package_type, properties.packages.single_message) +
             generate_section(properties.keys.target_type, parsedTextMessage.getTargetType()) +
             generate_section(properties.keys.target_id, parsedTextMessage.getTargetId()) +
             generate_section(properties.keys.target_module, parsedTextMessage.getModule()) +
             generate_section(properties.keys.response_type, parsedTextMessage.getResponseType()) +
             generate_section(properties.keys.request_id, parsedTextMessage.getRequestId());
#endif

        this->length_check(result);

        if(message.getPayloadType() == payload_type::text){
            result.append(message.getPayload());
            SEND(result, payload_type::text);
        } else if (message.getPayloadType() == payload_type::file_hath){
            std::ifstream file(message.getPayload());
            if (file.is_open()) {
                std::string file_string;
                std::stringstream ss;
                ss << file.rdbuf();
                file_string = ss.str();
                file.close();
                result.append(file_string);
                SEND(result, payload_type::binary_data);
            } else{
                std::cout << "Failed to read file!";
            }
        } else{
            result.append(message.getPayload());
            SEND(result, payload_type::binary_data);
        }
    }
}

CommandsManager::CommandsManager(cm::commands_manager_properties properties) : properties(std::move(properties)) {
    this->module_id = TARGET"CommandsManager";
    this->inboxMessagesHandler = std::thread(&CommandsManager::runInboxMessagesHandler, this);
    this->resultMessagesHandler = std::thread(&CommandsManager::runResultMessagesHandler, this);
}

//pthread_setname_np(pthread_self(),"command manager");
void CommandsManager::runInboxMessagesHandler() {
    std::pair<std::string, payload_type> inboxMessage;
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

void CommandsManager::runResultMessagesHandler() {
    std::pair<TaskResult, ParsedTextMessage> result;
    while (run){
        resultMessagesMutex.lock();
        if( ! resultMessages.empty()){
            result = resultMessages.front();
            resultMessages.pop();
            resultMessagesMutex.unlock();
            handleResponseMessage(result.first, result.second);
            std::cout << "handleRequestMessage(message)\n";
        } else{
            resultMessagesMutex.unlock();
            usleep(100);
        }
    }
}

void CommandsManager::stop_work() {
    this->run = false;
}

void CommandsManager::register_inbox_message(std::string &payload, payload_type pt) {
    inboxMessagesMutex.lock();
    this->inboxMessages.emplace(payload, pt);
    inboxMessagesMutex.unlock();
}

void CommandsManager::register_result_message(TaskResult &task, ParsedTextMessage &parsedMessage) {
    resultMessagesMutex.lock();
    this->resultMessages.emplace(std::make_pair(task,parsedMessage));
    resultMessagesMutex.unlock();
}

void CommandsManager::executeTask(std::string task, std::string payload, payload_type pt, std::function<void(payload_type, void *, bool)> callback) {

}

CommandsManager::~CommandsManager() {
    this->run = false;
    this->resultMessagesHandler.join();
    this->inboxMessagesHandler.join();
}

void CommandsManager::setMessageSizeGetter(std::function<int()> getter) {
    this->get_max_message_size = std::move(getter);
}