//
// Created by al_sah on 20.04.21.
//

#ifndef CORE_TASK_H
#define CORE_TASK_H

#include "ParsedTextMessage.h"
#include <string>

class Task {

private:
    std::string module;
    std::string request_id;
    std::string response_type;
public:

    Task(const std::string &module, const std::string &requestId, const std::string &responseType);
    Task(ParsedTextMessage *message);

    const std::string &getModule() const;
    void setModule(const std::string &module);
    const std::string &getRequestId() const;
    void setRequestId(const std::string &requestId);
    const std::string &getResponseType() const;
    void setResponseType(const std::string &responseType);



};


#endif //CORE_TASK_H
