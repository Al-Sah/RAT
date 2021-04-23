//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_MESSAGESMANAGER_H
#define BASIC_MODULE_MESSAGESMANAGER_H

#include <list>
#include "resources.h"
#include "ModulesManager.h"
#include "models/ParsedTextMessage.h"
#include "models/Task.h"

class CommandsManager {

private:

    CommandsManagerProperties properties;
    std::weak_ptr<ModulesManager> modulesManager;

    /// request_id, payload (temp)
    std::map<std::string, std::shared_ptr<std::string>> inboxTextMessagesBuffer;
    std::list<Task> tasks;

    void keyCheck(ParsedTextMessage *message, std::string &key, std::string &src);

public:

    CommandsManager(const CommandsManagerProperties &properties, std::weak_ptr<ModulesManager> modulesManager);

    ParsedTextMessage* parseMessage(const std::string& src);
    static std::string form_task(std::map<std::string, std::string> envelopeParams);
    static bool validate_parsed_message(ParsedTextMessage message);
    void handleMessage(const std::string& message);

};


#endif //BASIC_MODULE_MESSAGESMANAGER_H
