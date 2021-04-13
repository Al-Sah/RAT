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
