//
// Created by al_sah on 04.04.21.
//

#ifndef CORE_PARSEDTEXTMESSAGE_H
#define CORE_PARSEDTEXTMESSAGE_H


#include <map>

class ParsedTextMessage {

private:
    std::map<std::string, std::string> envelopeParams;
    std::string payload;
public:
    ParsedTextMessage(std::map<std::string, std::string> envelopeParams, std::string payload);

    const std::string &getPayload() const;

public:
    const std::map<std::string, std::string> &params() const;

};


#endif //CORE_PARSEDTEXTMESSAGE_H
