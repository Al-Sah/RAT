//
// Created by al_sah on 04.04.21.
//

#ifndef CORE_PARSED_TEXT_MESSAGE_H
#define CORE_PARSED_TEXT_MESSAGE_H


#include <map>

class ParsedTextMessage {

private:
    std::string payload;

    std::string target_module;
    std::string request_id;
    std::string package_type;
    std::string response_type;
    std::string full_payload_size;


    std::string target_type;
    std::string target_id;
    std::string is_last;

public:
    ParsedTextMessage() = default;
    explicit ParsedTextMessage(std::string payload);

    [[nodiscard]] const std::string &getPayload() const;
    void setPayload(const std::string &payload);

    [[nodiscard]] const std::string &getPackageType() const;
    void setPackageType(const std::string &packageType);

    [[nodiscard]] const std::string &getModule() const;
    void setModule(const std::string &module);

    [[nodiscard]] const std::string &getRequestId() const;
    void setRequestId(const std::string &requestId);

    [[nodiscard]] const std::string &getResponseType() const;
    void setResponseType(const std::string &responseType);

    [[nodiscard]] const std::string &getFullPayloadSize() const;
    void setFullPayloadSize(const std::string &fullPayloadSize);

    [[nodiscard]] const std::string &getTargetType() const;
    void setTargetType(const std::string &targetType);

    [[nodiscard]] const std::string &getTargetId() const;
    void setTargetId(const std::string &targetId);

    [[nodiscard]] const std::string &getIsLast() const;
    void setIsLast(const std::string &isLast);

};


#endif //CORE_PARSED_TEXT_MESSAGE_H
