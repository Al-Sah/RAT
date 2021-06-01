#include "Echo.h"
#include "echodialogue.h"


Echo::Echo(std::function<void(payload_type, void*, void*)> &callback, void* data){

    this->callback = callback;
    this->target = (QWidget*)data;

    this->moduleWindow = new EchoDialogue(this, target);
    this->module_id = "echo";

   connect(this, SIGNAL(openTarget(QString)), moduleWindow, SLOT(openBot(QString)));
   connect(this, SIGNAL(onMessage(QString, QString)), moduleWindow, SLOT(handleMessage(QString, QString)));

   connect(moduleWindow, SIGNAL(sendUIMessage(QString, QString)), this, SLOT(sendMessage(QString, QString)));
}



void Echo::executeTask(std::string task, std::string payload, payload_type pt, std::function<void(payload_type, void*, bool)> callback){

    this->tasks.remove(task);
    //QString temp = QString::fromStdString(payload.substr(0, payload.find(' ')));
    QString temp = QString::fromStdString(task);
    emit onMessage(temp, QString::fromStdString(payload.substr(payload.find(' ')+1)));
}

QWidget* Echo::getUI(QString targetId) {
    emit openTarget(targetId);
    return this->moduleWindow;
}

void Echo::sendMessage(QString botId, QString text){
    controlRequest info;

    info.target_type = targets_enum::bot;
    info.target_module = "echo";
    info.target_id = botId.toStdString();
    info.required_response = "one";

    std::string task = generate_uuid_v4();
    tasks.push_back(task);
    info.task_id = task;

    std::string payload = text.toStdString();
    callback(payload_type::text, &payload, &info);
}

