#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVector>

class Database
{
public:
    Database();
    ~Database();

    bool openDatabase();
    void closeDatabase();
    void addPhotoToDatabase();

    QVector<QPair<QVector<QString>, QPixmap>> getCars(QString queryStr);

    void addCar(const QString &name, const QString &price);

    void deleteCar(const QString &id_car);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
