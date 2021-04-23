//
// Created by al_sah on 20.04.21.
//

#include "../Task.h"

Task::Task(const std::string &module, const std::string &requestId, const std::string &responseType){
    this->module = module;
    this->response_type = responseType;
    this->request_id = requestId;
}

Task::Task(ParsedTextMessage *message){
    this->module = message->getModule();
    this->response_type = message->getResponseType();
    this->request_id = message->getRequestId();
}

const std::string &Task::getModule() const {
    return module;
}

void Task::setModule(const std::string &module) {
    Task::module = module;
}

const std::string &Task::getRequestId() const {
    return request_id;
}

void Task::setRequestId(const std::string &requestId) {
    request_id = requestId;
}

const std::string &Task::getResponseType() const {
    return response_type;
}

void Task::setResponseType(const std::string &responseType) {
    response_type = responseType;
}
