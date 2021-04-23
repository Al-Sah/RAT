//
// Created by al_sah on 04.04.21.
//

#ifndef CORE_PARSED_TEXT_MESSAGE_H
#define CORE_PARSED_TEXT_MESSAGE_H


#include <map>

class ParsedTextMessage {
private:
    std::map<std::string, std::string> envelopeParams;
    std::string payload;

    std::string module;
    std::string request_id;
    std::string package_type;
    std::string response_type;

public:
    ParsedTextMessage(std::map<std::string, std::string> envelopeParams, std::string payload);
    ParsedTextMessage(std::string payload);

    void setEnvelopeParams(const std::map<std::string, std::string> &envelopeParams);
    const std::string &getPayload() const;
    const std::map<std::string, std::string> &params() const;

    const std::string &getPackageType() const;
    void setPackageType(const std::string &packageType);
    const std::string &getModule() const;
    void setModule(const std::string &module);
    const std::string &getRequestId() const;
    void setRequestId(const std::string &requestId);
    const std::string &getResponseType() const;
    void setResponseType(const std::string &responseType);

};


#endif //CORE_PARSED_TEXT_MESSAGE_H
