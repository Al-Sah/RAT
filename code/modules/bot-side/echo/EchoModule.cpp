//
// Created by al_sah on 04.04.21.
//

#include <fstream>
#include "EchoModule.h"


EchoModule::EchoModule(std::function<void(payload_type, void*, void*)> &callback, void* data) {
    this->module_id = "echo";
}

std::string EchoModule::getId() const {
    return this->module_id;
}


void EchoModule::executeTask(std::string z, std::string payload, payload_type pt, std::function<void(payload_type, void*, bool)> callback) {

    if(pt == payload_type::text){
        payload.append("(Echo)");
        callback(payload_type::text, &payload, true);
    } else {
        std::ofstream file("/home/al_sah/Pictures/"+payload.substr(0, payload.find('0')));
        if (file.is_open()){
            file << payload.substr(payload.find('0')+1);
        }
        file.close();
        callback(payload_type::binary_data, &payload, true);
    }
}
