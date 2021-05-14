//
// Created by al_sah on 04.04.21.
//

#include "../ParsedTextMessage.h"

const std::string &ParsedTextMessage::getPayload() const {
    return payload;
}

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
    return target_module;
}

void ParsedTextMessage::setModule(const std::string &module) {
    ParsedTextMessage::target_module = module;
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

void ParsedTextMessage::setPayload(const std::string &payload) {
    ParsedTextMessage::payload = payload;
}

const std::string &ParsedTextMessage::getFullPayloadSize() const {
    return full_payload_size;
}

void ParsedTextMessage::setFullPayloadSize(const std::string &fullPayloadSize) {
    full_payload_size = fullPayloadSize;
}

const std::string &ParsedTextMessage::getTargetType() const {
    return target_type;
}

void ParsedTextMessage::setTargetType(const std::string &targetType) {
    target_type = targetType;
}

const std::string &ParsedTextMessage::getTargetId() const {
    return target_id;
}

void ParsedTextMessage::setTargetId(const std::string &targetId) {
    target_id = targetId;
}

const std::string &ParsedTextMessage::getIsLast() const {
    return is_last;
}

void ParsedTextMessage::setIsLast(const std::string &isLast) {
    is_last = isLast;
}

