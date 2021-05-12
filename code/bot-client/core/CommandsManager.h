//
// Created by al_sah on 09.03.21.
//

#ifndef BASIC_MODULE_MESSAGESMANAGER_H
#define BASIC_MODULE_MESSAGESMANAGER_H

#include <list>
#include "resources.h"
#include "models/ParsedTextMessage.h"
#include "models/Task.h"
#include "models/TaskResult.h"
#include "modules/Module.h"

#ifdef headers_includes
class WebsocketRunner;
class ModulesManager;
#endif

class CommandsManager : public Module { // TODO set command size limit (Implement temp storage)

private:
    volatile bool run = true;
    CommandsManagerProperties properties;

    std::mutex inboxMessagesMutex, resultMessagesMutex;

    /// parsing incoming messages, executing tasks by ModulesManager
    std::thread inboxMessagesHandler;
    /// parsing payload_type messages in order to send, (call WebSocketRunner)
    std::thread resultMessagesHandler;


    /// map <request_id, payload>     temporary payload storage ( Delete on task executing)
    std::map<std::string, std::shared_ptr<std::string>> inboxTextMessagesBuffer;
    /// request_id, payload_part      Ready to send payload
    std::map<std::string, std::map<int, std::shared_ptr<std::string>>> outboxTextMessagesBuffer;


    /// Tasks which are executing by modules (delete Task on last target_module response)
    std::list<Task> tasks;


    std::queue<std::string> inboxMessages; // WebSocketRunner put message here
    std::queue<TaskResult> resultMessages; // ModuleManager put message here


    static void length_check(std::string &raw_envelope); // TODO move to utils
    [[nodiscard]] std::string generate_section(std::string key, std::string value) const;

    // Inbox messages processing
    static std::string validate_parsed_message(ParsedTextMessage& message); // TODO move to utils
    std::string keyCheck(ParsedTextMessage *message, std::string &key, std::string &value) const;
    ParsedTextMessage* parseMessage(const std::string& src, std::string& errors);


    void handleRequestMessage(const std::string& message);
    void handleResponseMessage(TaskResult &message);

public:
    explicit CommandsManager(CommandsManagerProperties properties);
    void executeTask(std::string payload, payload_type pt, std::function<void (payload_type, void *, bool)> callback) override;

    void register_inbox_message(std::string& payload);
    void register_result_message(TaskResult& message);

    void runResultMessagesHandler(); // endless
    void runInboxMessagesHandler(); // endless

    void stop_work();



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
