//
// Created by al_sah on 28.05.21.
//

#include <iostream>
#include "serverInteraction.h"

serverInteraction::serverInteraction(std::function<void(payload_type, void*, void*)> &callback, void* data) {
    this->callback = callback;
    this->target = (QObject*)data;
    this->module_id = "serverInteraction";
}

void serverInteraction::getTargetsList() {

    controlRequest info;
    info.target_type = "S";
    info.target_module = "TL";
    info.target_id = "";


    std::string payload("getTargetsList");
    callback(text, &payload, &info);

}

void serverInteraction::executeTask(
        std::string payload, payload_type pt,
        std::function<void(payload_type, void*, bool)> callback) {

    std::cout << payload;
    getTargetsList();

}

void serverInteraction::do_targets_update() {
    getTargetsList();
}
