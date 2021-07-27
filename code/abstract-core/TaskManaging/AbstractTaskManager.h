//
// Created by al_sah on 09.03.21.
//
#ifndef MESSAGES_MANAGER_H
#define MESSAGES_MANAGER_H

#include "TaskManagerResources.h"
#include "InboxTasksRegister.h"
#include "TaskResultRegister.h"

// external interfaces
#include "../ModulesManager/TaskExecutingDelegator.h"
#include "../WebSocket/WebSocketMessageSender.h"


class AbstractTaskManager : public Module, public InboxTasksRegister, public TaskResultRegister { // TODO set command size limit (Implement temp storage)

protected:

    WebSocketMessageSender *messagesSender{};
    TaskExecutingDelegator *taskExecutor{};

    volatile bool run = true;
    cm::CommandsManagerProperties& properties;

    std::mutex inboxMessagesMutex, resultMessagesMutex;

    /// parsing incoming messages, executing tasks by AbstractModulesManager
    std::thread inboxMessagesHandler;
    /// parsing PayloadType messages in order to send, (call WebSocketRunner)
    std::thread resultMessagesHandler;

    /// map <requestId, payload>     temporary payload storage ( Delete on task executing)
    std::map<std::string, std::string> inboxTextMessagesBuffer;
    /// requestId, payload_part      Ready to send payload
    std::map<std::string, std::map<int, std::string>> outboxMessagesBuffer;


    /// Tasks which are executing by modules (delete TaskInfo on last targetModule response)
    std::list<TaskInfo> tasks;


    std::queue<std::pair<std::string, PayloadType>> inboxMessages; // WebSocketRunner put message here

    std::queue<std::pair<TaskResult,ParsedTextMessage>> resultMessages; // ModuleManager put message here
    // TODO resultMessages as TaskResult and ParsedMessage


    static void length_check(std::string &raw_envelope); // TODO move to utils
    [[nodiscard]] std::string generate_section(std::string key, std::string value) const;

    // Inbox messages processing
    std::string validate_parsed_message(ParsedTextMessage& message); // TODO move to utils
    std::string keyCheck(ParsedTextMessage *message, std::string &key, std::string &value) const;
    ParsedTextMessage* parseMessage(const std::string& src, std::string& errors);


    void handleRequestMessage(const std::string& message, PayloadType pt);
    virtual void handleTaskResult(TaskResult &message, ParsedTextMessage &parsedTextMessage) = 0;

public:
    explicit AbstractTaskManager(std::string &moduleId, std::string &version, cm::CommandsManagerProperties& properties);
    virtual ~AbstractTaskManager();

    void registerTaskResult(TaskResult &task, ParsedTextMessage &parsedMessage) override;
    void registerInboxTask(std::string& payload, PayloadType pt) override;

    void runResultMessagesHandler(); // endless
    void runInboxMessagesHandler(); // endless

    void setMessagesSender(WebSocketMessageSender *messagesSender);
    void setTaskExecutor(TaskExecutingDelegator *taskExecutor);

    void stop_work();
};

#endif //MESSAGES_MANAGER_H