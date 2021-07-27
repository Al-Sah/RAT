//
// Created by al_sah on 17.07.21.
//

#ifndef BOT_TASK_MANAGER
#define BOT_TASK_MANAGER


#include <TaskManaging/AbstractTaskManager.h>

class TaskManager : public AbstractTaskManager{

public:
    TaskManager(std::string moduleId, std::string version, cm::CommandsManagerProperties &properties);

    void handleTaskResult(TaskResult &message, ParsedTextMessage &parsedTextMessage) override;
};


#endif //BOT_TASK_MANAGER
