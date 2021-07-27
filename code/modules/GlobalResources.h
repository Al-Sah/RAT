//
// Created by al_sah on 12.05.21.
//

#ifndef CORE_MODULE_RESOURCES_H
#define CORE_MODULE_RESOURCES_H

#include <functional>
#include <sstream>
#include <random>

struct TargetsIds{
    std::string bot = "bot";
    std::string server = "server";
    std::string control = "control";
};

enum Targets{
    bot = 1,
    server = 2,
    control = 3
};

struct botResult{
    std::string task_id;
    bool isLast;
};

struct controlRequest{
    std::string task_id;
    std::string target_id;
    Targets target_type;
    std::string target_module;
    std::string required_response;
};

enum PayloadType{
    text = 1,
    filePath = 2,
    binaryData = 3
};



class TaskResult {
    std::string task_id;
    std::string payload;
    PayloadType payloadType{};
    bool isLast{};

public:

    PayloadType getPayloadType() const;
    TaskResult() = default;

    TaskResult(std::string &taskId, std::string &payload, PayloadType payloadType, bool isLast);
    TaskResult(std::string &payload, PayloadType payloadType, bool isLast);

    const std::string &getTaskId() const;
    const std::string &getPayload() const;
    bool getIsLast() const;
};

std::string generate_uuid_v4();

#endif //CORE_MODULE_RESOURCES_H
