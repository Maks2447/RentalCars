#ifndef REGISTRDIALOG_H
#define REGISTRDIALOG_H

#include "database.h"
#include "UserData.h"

#include <QDialog>
#include <QFrame>
#include <QLineEdit>

class MainWindow;

namespace Ui {
class RegistrDialog;
}

class RegistrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrDialog(QWidget *parent = nullptr);
    ~RegistrDialog();

    QFrame *line;

private slots:

    void on_changeButton_clicked();

    void on_acceptButton_clicked();

    void signIn();

    void createAccount();

    void applyStyleSheetDialog();

    void validators();

    bool emptyLineEdit(QWidget* parent);

    bool isRightPassword(QString password);

private:
    Ui::RegistrDialog *ui;
    bool currentButton;

    Database db;
    UserData currentUser;

    MainWindow *parentWindow;
};

#endif // REGISTRDIALOG_H
