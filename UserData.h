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

    void clear() {
        id_user = 0;
        name.clear();
        surname.clear();
        email.clear();
        phone.clear();
        password.clear();
    }
};

#endif // USERDATA_H
