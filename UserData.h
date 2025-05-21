#ifndef USERDATA_H
#define USERDATA_H

#include <QString>

struct UserData {
    int id_user = 1;
    QString name;
    QString surname;
    QString email;
    QString phone;
    QString password;
};

#endif // USERDATA_H
