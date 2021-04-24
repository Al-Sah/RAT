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
#include "WebsocketRunner.h"

//class WebsocketRunner;

class CommandsManager { // TODO set command size limit (Implement temp storage)

private:

    CommandsManagerProperties properties;
    std::weak_ptr<ModulesManager> modulesManager;
    std::weak_ptr<WebsocketRunner> websocketRunner;

    /// request_id, payload (temp)
    std::map<std::string, std::shared_ptr<std::string>> inboxTextMessagesBuffer;
    /// request_id, payload_part      Ready to send payload
//    std::map<std::string, std::map<int, std::shared_ptr<std::string>>> inboxTextMessagesBuffer;
    std::list<Task> tasks;

    void keyCheck(ParsedTextMessage *message, std::string &key, std::string &src) const;
    void length_check(std::string &raw_envelope); // TODO move to utils
    std::string generate_section(std::string key, std::string value, bool add_sd = false) const;

public:
    CommandsManager(CommandsManagerProperties properties);


    ParsedTextMessage* parseMessage(const std::string& src);
    static bool validate_parsed_message(ParsedTextMessage message); // TODO move to utils
    void handleRequestMessage(const std::string& message);
    void handleResponseMessage(std::string &task_id, std::shared_ptr<std::string> payload, bool isLast);

    void setModulesManager(const std::weak_ptr<ModulesManager> &modulesManager);
    void setWebsocketRunner(const std::weak_ptr<WebsocketRunner> &websocketRunner);

};


#endif //BASIC_MODULE_MESSAGESMANAGER_H
