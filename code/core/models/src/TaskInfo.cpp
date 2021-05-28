//
// Created by al_sah on 20.04.21.
//

#include "../TaskInfo.h"

TaskInfo::TaskInfo(const std::string &module, const std::string &requestId, const std::string &responseType){
    this->module = module;
    this->response_type = responseType;
    this->request_id = requestId;
}

TaskInfo::TaskInfo(ParsedTextMessage *message){
    this->module = message->getModule();
    this->response_type = message->getResponseType();
    this->request_id = message->getRequestId();
}

const std::string &TaskInfo::getModule() const {
    return module;
}

void TaskInfo::setModule(const std::string &module) {
    TaskInfo::module = module;
}

const std::string &TaskInfo::getRequestId() const {
    return request_id;
}

void TaskInfo::setRequestId(const std::string &requestId) {
    request_id = requestId;
}

const std::string &TaskInfo::getResponseType() const {
    return response_type;
}

void TaskInfo::setResponseType(const std::string &responseType) {
    response_type = responseType;
}
