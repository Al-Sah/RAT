//
// Created by al_sah on 28.05.21.
//


#include "ServerInteraction.h"
#include <string>
#include <iostream>
#include <sstream>
#include <QString>
#include <QDebug>

Q_DECLARE_METATYPE(TargetInfo)
Q_DECLARE_METATYPE(QList<TargetInfo>)

ServerInteraction::ServerInteraction(std::function<void(payload_type, void*, void*)> &callback, void* data) {

    this->callback = callback;
    this->target = (QWidget*)data;

    this->moduleWindow = new ServerInteractionGroupBox(this, target);

    connect(this, SIGNAL(sendTargetsToUI(QList<TargetInfo>, QList<TargetInfo>)),
        moduleWindow, SLOT(handleUpdatedTargets(QList<TargetInfo>, QList<TargetInfo>)));

    connect(this, SIGNAL(showModule(QString)), target, SLOT(showModule(QString)));

    this->module_id = "serverInteraction";

    qRegisterMetaType <TargetInfo>("TargetInfo");
    qRegisterMetaType <QList<TargetInfo>>("QList<TargetInfo>");
}


QWidget* ServerInteraction::getUI(){
    return this->moduleWindow;
}

void ServerInteraction::getTargetsList() {

    controlRequest info;
    info.target_type = targets_enum::server;
    info.target_module = "serverInteraction";
    info.target_id = "";
    
    std::string payload("getTargetsList");
    callback(text, &payload, &info);
}

void ServerInteraction::executeTask(
        std::string payload, payload_type pt,
        std::function<void(payload_type, void*, bool)> callback) {

    if(payload == "getTargetsList"){
        getTargetsList();
    } else {
        handleReceivedTargetsList(payload);
    }

}

void ServerInteraction::do_targets_update() {
    getTargetsList();
}

void ServerInteraction::showTargetModule(QListWidgetItem * info){
    emit showModule(info->text());
}

void ServerInteraction::handleReceivedTargetsList(std::string& payload) {

    QList<TargetInfo> botsList;
    QList<TargetInfo> usersList;
    auto activeList = &botsList;


    std::istringstream input;
    input.str(payload);
    for (std::string line; std::getline(input, line); ) {
        if(line == "bots list"){
            activeList = &botsList;
        } else if(line == "users list"){
            activeList = &usersList;
        } else{
            activeList->push_back(this->parseTarget(line));
        }
    }
    emit sendTargetsToUI(botsList, usersList);
}

TargetInfo ServerInteraction::parseTarget(std::string& line) {
    TargetInfo info;
    std::cout << std::endl << line << std::endl;
    std::string temp;
    size_t pos1 = line.find(' '), pos2;
    if(pos1 != std::string::npos){
        std::string id = line.substr(0, pos1);//line.substr(0, pos1);
        //std::cout << "ID" <<  id << std::flush;
        info.id = QString::fromStdString(id);
        std::cout << line.substr(0, pos1);
        qDebug() << info.id;
        ++pos1;
        line.erase(0, pos1);
    }

    pos1 = line.find('[');
    pos2 = line.find(']');
    temp = line.substr(pos1+1, pos2 - pos1);
    info.modules = parseTargetModules(temp);
    line.erase(0, pos2);

    pos1 = line.find('[');
    pos2 = line.find(']');
    temp = line.substr(pos1, pos2 - pos1 -1);
    info.characteristics = parseTargetCharacteristics(temp);

    return info;
}

QList<QString> ServerInteraction::parseTargetModules(std::string temp) {
    QList<QString> result;
    std::string delimiter = " ";

    size_t pos = 1;
    std::string token;
    while ((pos = temp.find(delimiter)) != std::string::npos) {
        token = temp.substr(0, pos);
        QString qTemp = QString::fromStdString(token);
        result.push_back(qTemp);
        qDebug() << qTemp;
        temp.erase(0, pos + delimiter.length());
    }
    return result;
}

QMap<QString, QString> ServerInteraction::parseTargetCharacteristics(std::string temp) {
    return QMap<QString, QString>();
}
