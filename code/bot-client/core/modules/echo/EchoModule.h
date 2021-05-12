//
// Created by al_sah on 04.04.21.
//

#ifndef BASIC_MODULE_ECHOMODULE_H
#define BASIC_MODULE_ECHOMODULE_H


#include "../Module.h"


class EchoModule : public Module {

public:
    EchoModule();

    [[nodiscard]] std::string getId() const override;
    //TaskResult executeTask(std::string payload) override;
    void executeTask(std::string payload, payload_type pt, std::function<void(payload_type, void*, bool)> callback) override;
};


#endif //BASIC_MODULE_ECHOMODULE_H
