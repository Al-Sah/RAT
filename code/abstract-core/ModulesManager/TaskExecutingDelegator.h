//
// Created by al_sah on 16.07.21.
//

#ifndef ABSTRACT_CORE_TASKEXECUTINGDELEGATOR_H
#define ABSTRACT_CORE_TASKEXECUTINGDELEGATOR_H

class TaskExecutingDelegator {
public:
    virtual void handleTask(std::string& module, std::string& task_id, std::string& payload, PayloadType pt) =0;
};

#endif //ABSTRACT_CORE_TASKEXECUTINGDELEGATOR_H