//
// Created by al_sah on 30.05.21.
//

#ifndef SERVER_INTERACTION_RECOURSES_H
#define SERVER_INTERACTION_RECOURSES_H

#include <QString>
#include <QMap>

struct TargetInfo{
    QString id;
    QList<QString> modules;
    QMap<QString, QString> characteristics;
};

#endif //SERVER_INTERACTION_RECOURSES_H
