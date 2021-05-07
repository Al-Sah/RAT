//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_MESSAGESMANAGER_H
#define BASIC_MODULE_MESSAGESMANAGER_H

#include <list>
#include "resources.h"
#include "models/ParsedTextMessage.h"
#include "models/Task.h"

#ifdef headers_includes
class WebsocketRunner;
class ModulesManager;
#endif

class CommandsManager { // TODO set command size limit (Implement temp storage)

private:

    std::thread ws_listener;

    CommandsManagerProperties properties;


    /// task_id, payload (temp)
    std::map<std::string, std::shared_ptr<std::string>> inboxTextMessagesBuffer;
    /// task_id, payload_part      Ready to send payload
//    std::map<std::string, std::map<int, std::shared_ptr<std::string>>> inboxTextMessagesBuffer;
    std::list<Task> tasks;




    std::queue<std::string> inboxMessages;



    void keyCheck(ParsedTextMessage *message, std::string &key, std::string &src) const;
    void length_check(std::string &raw_envelope); // TODO move to utils
    std::string generate_section(std::string key, std::string value) const;

public:
    CommandsManager(CommandsManagerProperties properties);


    ParsedTextMessage* parseMessage(const std::string& src);
    static bool validate_parsed_message(ParsedTextMessage message); // TODO move to utils
    void handleRequestMessage(const std::string& message);
    void handleResponseMessage(std::string &task_id, std::shared_ptr<std::string> payload, bool isLast);


    void add_new_message(std::string& payload);


    [[noreturn]] void run_ws_listener(); // endless



#ifdef headers_includes
    private:
    std::weak_ptr<ModulesManager> modulesManager;
    std::weak_ptr<WebsocketRunner> websocketRunner;
public:
    void setModulesManager(const std::weak_ptr<ModulesManager> &modulesManager);
    void setWebsocketRunner(const std::weak_ptr<WebsocketRunner> &websocketRunner);
#else
    std::function<bool(std::string)> send_message_function;
    std::function<void(std::string, std::string, std::shared_ptr<std::string>)> task_executor;
    void setMessageSender(std::function<bool(std::string)>& sender);
    void setTaskExecutor(std::function<void(std::string, std::string, std::shared_ptr<std::string>)>& task_executor);
#endif
};


#endif //BASIC_MODULE_MESSAGESMANAGER_H
