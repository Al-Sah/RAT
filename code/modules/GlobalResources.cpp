//
// Created by al_sah on 01.06.21.
//

//#include "module_resources.h"


#include "GlobalResources.h"


TaskResult::TaskResult(std::string &taskId, std::string &payload, PayloadType payloadType, bool isLast)
        : task_id(taskId),
          payload(payload),
          payloadType(payloadType),
          isLast(isLast) {}

TaskResult::TaskResult(std::string &payload, PayloadType payloadType, bool isLast)
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

PayloadType TaskResult::getPayloadType() const {
    return payloadType;
}



std::string generate_uuid_v4() {
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    };
    return ss.str();
}

