//
// Created by al_sah on 04.04.21.
//

#ifndef BASIC_MODULE_PICTURES_TAKER_H
#define BASIC_MODULE_PICTURES_TAKER_H


#include "../../Module.h"


class PicturesTaker : public Module {

public:
    PicturesTaker(std::function<void(payload_type, void*, void*)> &callback, void* data);

    [[nodiscard]] std::string getId() const override;
    void executeTask(std::string task, std::string payload, payload_type pt, std::function<void(payload_type, void*, bool)> callback) override;
};


#endif //BASIC_MODULE_PICTURES_TAKER_H
