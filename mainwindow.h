#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database.h"
#include "registrdialog.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QCalendarWidget>
#include <QFormLayout>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadCars(QString queryStr);

    void setData(const UserData &user);

private slots:

    void resetFilters_clicked();

    void on_LoginButton_clicked();

    void logout();

    void on_setFilters_pushButton_clicked();

    QString getFilters();

    void clearCars(QLayout *layout);

    void applyStyleSheet();

    void orderCarShow(const QVector<QString>& carData, const QPixmap &photoPixmap);

    QPixmap roundedPixmap(const QPixmap photoPixmap, int width, int height);

    void on_Home_DayFrom_pushButton_clicked();

    void calendarDataChoice(const QDate &date);

    bool eventFilter(QObject *watched, QEvent *event) override;

    void on_Home_DayTo_pushButton_clicked();

    void addRowsTime(QMenu &menu, QToolButton *name);

    void Home_search_pushButton_clicked(QString query);

    void on_Order_button_clicked();

    void set_validator();

    void show_active_orders();

    void show_past_orders();

    void create_widgetCard(QVBoxLayout *layoutWidgetCard, QVector<QPair<QVector<QString>, QPixmap>> &carsList, bool isActive);

    void creationInnerTabs();

    void saveChangesButton();

    void refreshData();

    void changeCurrentTab();

    void on_add_new_car_button_clicked();

    void delete_car_Tab();

    void create_widget_delete_change(QVector<QPair<QVector<QString>, QPixmap>> carsList);

    void on_plus_upload_Photo_button_clicked();

    void change_car_info(const QVector<QString> &car);

    void delete_car(const QVector<QString> &car, QWidget *widgetCard);

    void on_saveChanges_car_button_clicked();

    void create_unverified_orders_widgets();

    void verificate_car_button(const QVector<QString> &car);

    void on_confirm_verification_button_clicked();

    void on_logout_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    Database db;
    UserData currentUser;

    QWidget *container;
    QVBoxLayout *layout;

    //UserData user;
    QMenu *menu;

    QCalendarWidget *calendar;

    bool isLogin;

    QDate calendar_start_date;
    QDate calendar_end_date;
    QDate start_date;
    QDate end_date;

    QString timePick_start;
    QString timePick_end;

    QString car_id;
    QString car_id_change;

    QString oldStyle;

    QPushButton *saveChangesButton_personalInformation = nullptr;
    QPushButton *saveChangesButton_email = nullptr;
    QPushButton *saveChangesButton_changePassword = nullptr;

    QLabel *isSuccessful;

    QString currentPhotoFilePath;

    bool isFiltr;
    bool myBookingsTab;
};
#endif // MAINWINDOW_H
