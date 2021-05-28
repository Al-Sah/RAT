//
// Created by al_sah on 11.05.21.
//

#ifndef CORE_TASKRESULT_H
#define CORE_TASKRESULT_H


#include <string>
#include "payload_type.h"

class TaskResult {
    std::string task_id;
    std::string payload;
    payload_type payloadType;
public:
    payload_type getPayloadType() const;

private:
    bool isLast;

public:
    TaskResult() = default;
    TaskResult(std::string &taskId, std::string &payload, payload_type payloadType, bool isLast);
    TaskResult(std::string &payload, payload_type payloadType, bool isLast);

    const std::string &getTaskId() const;
    const std::string &getPayload() const;
    bool getIsLast() const;
};


#endif //CORE_TASKRESULT_H
