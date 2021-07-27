//
// Created by al_sah on 16.07.21.
//

#ifndef ABSTRACT_CORE_RESULT_TASKS_REGISTER
#define ABSTRACT_CORE_RESULT_TASKS_REGISTER

/** @interface TaskResultRegister.
 *
 * TaskResultRegister is an interface which is used for registering
 * results of executed Tasks by modules
 *
 * This interface is implemented by @class AbstractTaskManager,
 * used in @class AbstractModulesManager
 **/

class TaskResultRegister {
public:
    virtual void registerTaskResult(TaskResult &task, ParsedTextMessage &parsedMessage) =0;
};


#endif //ABSTRACT_CORE_RESULT_TASKS_REGISTER