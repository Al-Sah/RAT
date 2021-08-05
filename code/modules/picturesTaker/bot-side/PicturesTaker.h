//
// Created by al_sah on 04.04.21.
//

#ifndef BASIC_MODULE_PICTURES_TAKER_H
#define BASIC_MODULE_PICTURES_TAKER_H


#include "TaskExecutor.h"


class PicturesTaker : public TaskExecutor{

public:
    PicturesTaker(std::function<void(PayloadType, void*, void*)> &callback, void* data);

    void executeTask(std::string task, std::string payload, PayloadType pt, std::function<void(PayloadType, void*, bool)> callback) override;
};


#endif //BASIC_MODULE_PICTURES_TAKER_H
