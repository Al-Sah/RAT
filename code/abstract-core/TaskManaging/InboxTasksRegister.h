//
// Created by al_sah on 16.07.21.
//

#ifndef ABSTRACT_CORE_INBOX_TASKS_REGISTER_INTERFACE
#define ABSTRACT_CORE_INBOX_TASKS_REGISTER_INTERFACE

/** @interface InboxTasksRegister.
 *
 * InboxTasksRegister is an interface which is used for registering
 * in a queue new tasks, received from server.
 *
 * This interface is implemented by @class AbstractTaskManager,
 * used in @class AbstractTaskManager
 **/

#include "GlobalResources.h"

class InboxTasksRegister {
public:
    /**
    * multithreading-safe method
    * @param1 - raw payload
    * @param2 - payload type (binary or text)
    **/
    virtual void registerInboxTask(std::string&, PayloadType)=0;
};

#endif //ABSTRACT_CORE_INBOX_TASKS_REGISTER_INTERFACE