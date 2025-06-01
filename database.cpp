#include "database.h"
#include "qpixmap.h"

#include <QSqlQuery>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QSqlError>
#include <QDate>

Database::Database() {

}

Database::~Database() {
    closeDatabase();
}

bool Database::openDatabase() {

    db = QSqlDatabase::addDatabase("QPSQL");

    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("RentalCars");
    db.setUserName("postgres");
    db.setPassword("45456565Maksim_");

    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных!";
        return false;
    }
    return true;
}

void Database::closeDatabase() {
    db.close();
}

void Database::addPhotoToDatabase()
{
    QFile ImgFile("C:/Users/golov/Downloads/Volkswagen_Arteon.jpg");
    if (!ImgFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Не удалось открыть файл изображения!";
        return;
    }
    QByteArray imageData = ImgFile.readAll();

    ImgFile.close();

    QSqlQuery query;
    query.prepare("UPDATE \"Cars\" SET \"photo\" = :photo WHERE \"id_car\" = :id_car");
    query.bindValue(":photo", imageData);
    query.bindValue(":id_car", 0);

    if (!query.exec()) {
        qDebug() << "Ошибка при вставке данных в базу:" << query.lastError().text();
    } else {
        qDebug() << "Изображение успешно добавлено в базу данных!";
    }
}

QVector<QPair<QVector<QString>, QPixmap>> Database::getCars(QString queryStr)
{
    QVector<QPair<QVector<QString>, QPixmap>> carList;
    QSqlQuery query;

    if(queryStr.isEmpty()) {
        query.prepare("SELECT \"photo\", \"model\", \"specifications\", \"class\", \"fuel\", \"transmission\", \"price\", \"type\", \"passengers\", \"id_car\" "
                      "FROM \"Cars\" "
                      "WHERE \"id_deleted\" = false ");
    } else {
        queryStr += " ORDER BY \"price\" DESC";
        query.prepare(queryStr);
    }

    if(query.exec()) {
        while(query.next()) {
            QVector<QString> car;

            qDebug() << query.value("price");
            car.append(query.value("model").toString());
            car.append(query.value("specifications").toString());
            car.append(query.value("class").toString());
            car.append(query.value("fuel").toString());
            car.append(query.value("transmission").toString());
            car.append(query.value("price").toString());
            car.append(query.value("type").toString());
            car.append(query.value("passengers").toString());
            car.append(query.value("id_car").toString());

            QPixmap pixmap;
            QByteArray photoData = query.value("photo").toByteArray();

            if(!photoData.isEmpty()) {
                pixmap.loadFromData(photoData);
            }

            carList.append(qMakePair(car, pixmap));
        }
    }
    return carList;
}

void Database::deleteCar(const QString &id_car)
{
    QSqlQuery query;
    query.prepare("UPDATE \"Cars\" SET \"is_deleted\" = TRUE "
                  "WHERE \"id_car\" = :id_car");
    query.bindValue(":id_car", id_car);

    if(query.exec()) {
        qDebug() << "Deleted";
    } else {
        qDebug() << "Not deleted!!" << query.lastError().text();
    }
}

QVector<QPair<QVector<QString>, QPixmap>> Database::unverified_orders()
{
    QSqlQuery query;
    QDate currentDate = QDate::currentDate();
    qDebug() << currentDate;
    query.prepare("SELECT \"Orders\".\"id_order\", \"Orders\".\"start_date\", \"Orders\".\"end_date\", \"Cars\".\"photo\", \"Cars\".\"model\", "
                  "\"Users\".\"name\", \"Users\".\"surname\", \"Users\".\"email\" "
                  "FROM \"Orders\" "
                  "JOIN \"Cars\" ON \"Orders\".\"car_id\" = \"Cars\".\"id_car\" "
                  "JOIN \"Users\" ON \"Orders\".\"user_id\" = \"Users\".\"id_user\" "
                  "WHERE \"Orders\".\"end_date\" < :currentDate "
                  "AND \"Orders\".\"checked\" = false ");
    query.bindValue(":currentDate", currentDate);

    QVector<QPair<QVector<QString>, QPixmap>> ordersList;

    if(query.exec()) {
        while(query.next()) {
            QVector<QString> car;

            car.append(query.value("id_order").toString());
            car.append(query.value("start_date").toDate().toString("dd MMM yyyy"));
            car.append(query.value("end_date").toDate().toString("dd MMM yyyy"));
            car.append(query.value("model").toString());
            car.append(query.value("name").toString());
            car.append(query.value("surname").toString());
            car.append(query.value("email").toString());

            QPixmap pixmap;
            QByteArray photoData = query.value("photo").toByteArray();

            if(!photoData.isEmpty()) {
                pixmap.loadFromData(photoData);
            }

            ordersList.append(qMakePair(car, pixmap));
        }
    }
    return ordersList;
}
