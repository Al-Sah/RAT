//
// Created by al_sah on 20.04.21.
//

#ifndef CORE_TASKINFO_H
#define CORE_TASKINFO_H

#include "ParsedTextMessage.h"
#include <string>

class TaskInfo {

private:
    std::string module;
    std::string request_id;
    std::string response_type;
public:

    TaskInfo(const std::string &module, const std::string &requestId, const std::string &responseType);
    TaskInfo(ParsedTextMessage *message);

    const std::string &getModule() const;
    void setModule(const std::string &module);
    const std::string &getRequestId() const;
    void setRequestId(const std::string &requestId);
    const std::string &getResponseType() const;
    void setResponseType(const std::string &responseType);



};


#endif //CORE_TASKINFO_H
