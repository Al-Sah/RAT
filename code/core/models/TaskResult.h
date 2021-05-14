//
// Created by al_sah on 11.05.21.
//

#ifndef CORE_TASKRESULT_H
#define CORE_TASKRESULT_H


#include <string>

class TaskResult {
    std::string task_id;
    std::string payload;
    bool isFile;
    bool isLast;

public:
    TaskResult() = default;
    TaskResult(std::string &taskId, std::string &payload, bool isFile, bool isLast);
    TaskResult(std::string &payload, bool isFile, bool isLast);

    const std::string &getTaskId() const;
    const std::string &getPayload() const;
    bool getIsLast() const;
    bool getIsFile() const;
};


#endif //CORE_TASKRESULT_H
