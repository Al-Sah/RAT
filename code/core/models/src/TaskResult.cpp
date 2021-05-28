//
// Created by al_sah on 11.05.21.
//

#include "../TaskResult.h"

TaskResult::TaskResult(std::string &taskId, std::string &payload, payload_type payloadType, bool isLast)
        : task_id(taskId),
          payload(payload),
          payloadType(payloadType),
          isLast(isLast) {}

TaskResult::TaskResult(std::string &payload, payload_type payloadType, bool isLast)
        : payload(payload),
          payloadType(payloadType),
          isLast(isLast) {}


const std::string &TaskResult::getTaskId() const {
    return task_id;
}
const std::string &TaskResult::getPayload() const {
    return payload;
}
bool TaskResult::getIsLast() const {
    return isLast;
}

payload_type TaskResult::getPayloadType() const {
    return payloadType;
}

