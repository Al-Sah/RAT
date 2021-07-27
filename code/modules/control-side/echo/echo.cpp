#include "Echo.h"
#include "echodialogue.h"
#include <fstream>


Echo::Echo(std::function<void(PayloadType, void*, void*)> &callback, void* data){

    this->moduleId = "echo";
    this->version = "1.0.0";

    this->callback = callback;
    this->target = (QWidget*)data;

    this->moduleWindow = new EchoDialogue(this, target);


    connect(this, SIGNAL(openTarget(QString)), moduleWindow, SLOT(openBot(QString)));
    connect(this, SIGNAL(onMessage(QString, QString)), moduleWindow, SLOT(handleMessage(QString, QString)));
    connect(this, SIGNAL(onFile(QString, QString)), moduleWindow, SLOT(handleFile(QString, QString)));

    connect(moduleWindow, SIGNAL(sendUIMessage(QString, QString)), this, SLOT(sendMessage(QString, QString)));
    connect(moduleWindow, SIGNAL(sendUIFile(QString, QString)), this, SLOT(sendFile(QString, QString)));
}



void Echo::executeTask(std::string task, std::string payload, PayloadType pt, std::function<void(PayloadType, void*, bool)> callback){


    QString temp = QString::fromStdString(task);
    if(pt == PayloadType::text){
        emit onMessage(QString::fromStdString(tasks[task]), QString::fromStdString(payload.substr(payload.find(' ')+1)));
    } else if(pt == PayloadType::binaryData){

    std::ofstream file("/home/al_sah/Pictures/"+payload.substr(0, payload.find('0'))+"(Echo result)");
        if (file.is_open()){
            file << payload.substr(payload.find('0')+1);
        }
        file.close();
        emit onFile(QString::fromStdString(tasks[task]), QString::fromStdString(payload.substr(0, payload.find('0')))+"(Echo result)");
    }
     this->tasks.erase(tasks.find(task));
}

QWidget* Echo::getUI(QString targetId) {
    emit openTarget(targetId);
    return this->moduleWindow;
}

void Echo::sendFile(QString botId, QString path){
    controlRequest info;

    info.target_type = Targets::bot;
    info.target_module = "echo";
    info.target_id = botId.toStdString();
    info.required_response = "one";

    std::string task = generate_uuid_v4();
    tasks.insert(tasks.begin(), std::make_pair(info.target_id, task));
    info.task_id = task;

    std::string result = path.toStdString();
    result = result.substr(result.find_last_of('/')+1);
    result+='0';


    std::ifstream file(path.toStdString());
    if (file.is_open()) {
        std::string file_string;
        std::stringstream ss;
        ss << file.rdbuf();
        file_string = ss.str();
        file.close();
        result.append(file_string);
    }
    callback(PayloadType::binaryData, &result, &info);
}

void Echo::sendMessage(QString botId, QString text){
    controlRequest info;

    info.target_type = Targets::bot;
    info.target_module = "echo";
    info.target_id = botId.toStdString();
    info.required_response = "one";

    std::string task = generate_uuid_v4();
    tasks.insert(tasks.begin(), std::make_pair(info.target_id, task));
    info.task_id = task;

    std::string payload = text.toStdString();
    callback(PayloadType::text, &payload, &info);
}