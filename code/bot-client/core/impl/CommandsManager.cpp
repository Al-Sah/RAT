//
// Created by al_sah on 09.03.21.
//

#include <search.h>
#include "../CommandsManager.h"

void CommandsManager::handleMessage(const std::string& src) {

    ParsedTextMessage* message = parseMessage(src);
    std::string pack_type = message->getPackageType();
    std::string request_id = message->getRequestId();
    // extracting payload and registering the task
/*    if(pack_type == "first" || pack_type == "single"){
        inboxTextMessagesBuffer.emplace(request_id, std::make_shared<std::string>(message->getPayload()));
        tasks.push_back(*(new Task(message)));
    } else if (pack_type == "con" || pack_type == "last"){
        inboxTextMessagesBuffer.find(request_id)->second->append( message->getPayload());
    }

    if(pack_type == "single" || pack_type == "last"){
        modulesManager.lock()
        ->executeTask(message->getModule(), request_id, (inboxTextMessagesBuffer.find(request_id))->second);
        // TO DO execute command and remove message from buffer
        std::cout << inboxTextMessagesBuffer.find(message->getRequestId())->second;
    }*/

    if(pack_type == "first"){
        inboxTextMessagesBuffer.emplace(request_id, std::make_shared<std::string>(message->getPayload()));
        if(message->getResponseType() != "none"){
            tasks.push_back(*(new Task(message)));
        }
    } else if (pack_type == "con"){
        inboxTextMessagesBuffer.find(request_id)->second->append( message->getPayload());
    }  else if (pack_type == "last"){
        inboxTextMessagesBuffer.find(request_id)->second->append( message->getPayload());
        modulesManager.lock()->executeTask(
                        message->getModule(),
                        request_id,
                        (inboxTextMessagesBuffer.find(request_id))->second);
        inboxTextMessagesBuffer.erase(request_id);
    }

    if(pack_type == "single"){
        if(message->getResponseType() != "none"){
            tasks.push_back(*(new Task(message)));
        }
        modulesManager.lock()->executeTask(
                        message->getModule(),
                        request_id,
                        std::make_shared<std::string>(message->getPayload()));
    }


}

ParsedTextMessage* CommandsManager::parseMessage(const std::string &src) {

    std::string key, temp, envelope, payload;
    std::map<std::string, std::string> envelopeParams;

    std::string::size_type position = src.find_first_of(':');
    uint envelope_size = std::stoi(src.substr(0, position));
    envelope = src.substr(position + 1, envelope_size - position -1 );
    auto *parsedTextMessage = new ParsedTextMessage(src.substr(envelope_size));

    if(properties.enable_args_id){
        for(char ch : envelope){
            if(ch == properties.delimiters.id_val){
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

std::string CommandsManager::form_task(std::map<std::string, std::string> envelopeParams) {

    return std::string();
}

bool CommandsManager::validate_parsed_message(ParsedTextMessage message) {
//    message.params()
/*

    std::string request_id = message.params().find("REQ") != parsed->params().end() ? parsed->params().find("REQ")->second : "";
    std::string package_type = parsed->params().find("PT") != parsed->params().end() ? parsed->params().find("PT")->second : "";
    std::string module = parsed->params().find("M") != parsed->params().end() ? parsed->params().find("M")->second : "";
*/


    return false;
}

CommandsManager::CommandsManager(const CommandsManagerProperties &properties, std::weak_ptr<ModulesManager> modulesManager) {
    this->properties = properties;
    this->modulesManager = modulesManager;
}

void CommandsManager::keyCheck(ParsedTextMessage *message, std::string &key, std::string &src) {
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
