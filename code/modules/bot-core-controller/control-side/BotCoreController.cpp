//
// Created by al_sah on 05.08.21.
//

#include "BotCoreController.h"
#include "ModuleDialogue.h"

BotCoreController::BotCoreController(std::function<void(PayloadType, void *, void *)> &callback, void *target) {
    this->moduleId = "RatBotCore";
    this->callback = callback;

   this->moduleWindow = new ModuleDialogue(this, (QWidget*) target);

}

void BotCoreController::executeTask(std::string task, std::string payload, PayloadType pt,
                                    std::function<void(PayloadType, void *, bool)> callback) {

}

QWidget *BotCoreController::getUI(QString targetId) {
    return moduleWindow;
}
