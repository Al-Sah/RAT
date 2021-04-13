//
// Created by al_sah on 09.03.21.
//

#include "../MessagesManager.h"

void MessagesManager::handleMessage(const std::string& message) {

    ParsedTextMessage* parsed = parseMessage(message);

    std::string request_id = parsed->params().find("REQ") != parsed->params().end() ? parsed->params().find("REQ")->second : "";
    std::string package_type = parsed->params().find("PT") != parsed->params().end() ? parsed->params().find("PT")->second : "";
    std::string module = parsed->params().find("M") != parsed->params().end() ? parsed->params().find("M")->second : "";


    if(package_type == "first" || package_type == "single"){
        textMessagesBuffer.emplace(request_id, parsed->getPayload());
//        modulesManager->register_task();
        //tasksList.emplace_back(request_id, module);
    } else if (package_type == "con" || package_type == "last"){
        textMessagesBuffer.find(request_id)->second += parsed->getPayload();
    }

    if(package_type == "single" || package_type == "last"){
        // TODO execute command and remove message from buffer
        std::cout << textMessagesBuffer.find(request_id)->second;
    }


}

ParsedTextMessage* MessagesManager::parseMessage(const std::string &src) {
    std::string key, temp, envelope, payload;
    std::map<std::string, std::string> envelopeParams;

    std::string::size_type pos = src.find_first_of(':');
    uint envelope_size = std::stoi(src.substr(0, pos));
    envelope = src.substr(pos+1,envelope_size);

    for(char ch : envelope){
        if(ch == ':'){
            key = temp;
            temp.clear();
        } else if(ch == '#'){
            envelopeParams.emplace(key,temp);
            temp.clear();
        } else{
            temp += ch;
        }
    }
    return new ParsedTextMessage(envelopeParams, src.substr(envelope_size));
}

std::string MessagesManager::form_task(std::map<std::string, std::string> envelopeParams) {

    return std::string();
}

bool MessagesManager::validate_parsed_message(ParsedTextMessage message) {
//    message.params()
/*

    std::string request_id = message.params().find("REQ") != parsed->params().end() ? parsed->params().find("REQ")->second : "";
    std::string package_type = parsed->params().find("PT") != parsed->params().end() ? parsed->params().find("PT")->second : "";
    std::string module = parsed->params().find("M") != parsed->params().end() ? parsed->params().find("M")->second : "";
*/


    return false;
}



/*std::string MessagesManager::decrypt(std::string& toDecrypt) const {
    if(encryption == 0){
        return toDecrypt;
    } else {
        return toDecrypt += " decrypted"; //TODO  (._.)
    }

}

std::string MessagesManager::encrypt(std::string& toEncrypt) const{
    return std::string();
}*/



/*
for(int i = 0; i <= src.size(); ++i){
if(src[i] == '\"'){
if(!keyReading && !valueReading){
if(key.empty()){
keyReading = true;
} else {
valueReading = true;
}
} else if(keyReading) {
if(src[i+1] != ':'){
temp += src[i];
} else{
key = temp;
temp.clear();
keyReading = false;
}
} else {
if(src.size() > i+1 && src[i+1] != ','){
temp += src[i];
} else {
value = temp;
temp.clear();
valueReading = false;
}
}
continue;
} else if(src[i] == ','){
if(keyReading || valueReading){
temp += src[i];
} else {
this->messageParts.insert({key,value});
key.clear(); value.clear();
}
}
else {
if(keyReading || valueReading){
temp += src[i];
} else {
continue;
}
}
}
if(!key.empty() && !value.empty()){
this->messageParts.insert({key,value});
}*/
