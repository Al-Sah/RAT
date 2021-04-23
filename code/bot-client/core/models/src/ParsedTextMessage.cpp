//
// Created by al_sah on 04.04.21.
//

#include "../ParsedTextMessage.h"

#include <utility>

const std::map<std::string, std::string> &ParsedTextMessage::params() const {
    return envelopeParams;
}

const std::string &ParsedTextMessage::getPayload() const {
    return payload;
}

ParsedTextMessage::ParsedTextMessage(
    std::map<std::string, std::string> envelopeParams,
    std::string payload) : envelopeParams(std::move(envelopeParams)), payload(std::move(payload)) {}

ParsedTextMessage::ParsedTextMessage(std::string payload) {
    this->payload = std::move(payload);
}

const std::string &ParsedTextMessage::getPackageType() const {
    return package_type;
}

void ParsedTextMessage::setPackageType(const std::string &packageType) {
    package_type = packageType;
}

const std::string &ParsedTextMessage::getModule() const {
    return module;
}

void ParsedTextMessage::setModule(const std::string &module) {
    ParsedTextMessage::module = module;
}

const std::string &ParsedTextMessage::getRequestId() const {
    return request_id;
}

void ParsedTextMessage::setRequestId(const std::string &requestId) {
    request_id = requestId;
}

const std::string &ParsedTextMessage::getResponseType() const {
    return response_type;
}

void ParsedTextMessage::setResponseType(const std::string &responseType) {
    response_type = responseType;
}

void ParsedTextMessage::setEnvelopeParams(const std::map<std::string, std::string> &envelopeParams) {
    ParsedTextMessage::envelopeParams = envelopeParams;
}

