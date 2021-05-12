//
// Created by al_sah on 11.05.21.
//

#include "../TaskResult.h"

TaskResult::TaskResult(std::string &taskId, std::string &payload, bool isFile, bool isLast)
        : task_id(taskId),
          payload(payload),
          isFile(isFile),
          isLast(isLast) {}

TaskResult::TaskResult(std::string &payload, bool isFile, bool isLast)
        : payload(payload),
          isFile(isFile),
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
bool TaskResult::getIsFile() const {
    return isFile;
}

