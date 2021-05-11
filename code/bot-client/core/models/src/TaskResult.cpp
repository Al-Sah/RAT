//
// Created by al_sah on 11.05.21.
//

#include "../TaskResult.h"

const std::string &TaskResult::getTaskId() const {
    return task_id;
}

const std::string &TaskResult::getPayload() const {
    return payload;
}

bool TaskResult::getIsLast() const {
    return isLast;
}

TaskResult::TaskResult(std::string &taskId, std::string &payload, bool isLast)
    : task_id(taskId),
    payload(payload),
    isLast(isLast) {}

