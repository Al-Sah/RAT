//
// Created by al_sah on 04.04.21.
//

#include <fstream>
#include "EchoModule.h"


EchoModule::EchoModule(std::function<void(PayloadType, void*, void*)> &callback, void* data) {
    this->moduleId = "echo";
}


void EchoModule::executeTask(std::string z, std::string payload, PayloadType pt, std::function<void(PayloadType, void*, bool)> callback) {

    if(pt == PayloadType::text){
        payload.append("(Echo)");
        callback(PayloadType::text, &payload, true);
    } else {
        std::ofstream file("/home/al_sah/Pictures/"+payload.substr(0, payload.find('0')));
        if (file.is_open()){
            file << payload.substr(payload.find('0')+1);
        }
        file.close();
        callback(PayloadType::binaryData, &payload, true);
    }
}

