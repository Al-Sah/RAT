//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_MESSAGESMANAGER_H
#define BASIC_MODULE_MESSAGESMANAGER_H

#include <list>
#include "resources.h"
#include "ModulesManager.h"
#include "ParsedTextMessage.h"

class MessagesManager {
private:
    std::shared_ptr<ModulesManager> modulesManager;
    /// request_id, payload
    std::map<std::string, std::string> textMessagesBuffer;



public:

    static ParsedTextMessage* parseMessage(const std::string& src);
    static std::string form_task(std::map<std::string, std::string> envelopeParams);
    static bool validate_parsed_message(ParsedTextMessage message);
    void handleMessage(const std::string& message);
};


#endif //BASIC_MODULE_MESSAGESMANAGER_H
