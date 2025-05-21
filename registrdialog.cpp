#include "registrdialog.h"
#include "ui_registrdialog.h"
#include "UserData.h"
#include "mainwindow.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include <QFileDialog>
#include <QRegularExpressionValidator>
#include <QLineEdit>
#include <QMessageBox>

RegistrDialog::RegistrDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegistrDialog)
    , currentButton(true)
    , parentWindow(static_cast<MainWindow*>(parent))
{
    ui->setupUi(this);

    this->setObjectName("this");
    ui->acceptButton->setObjectName("acceptButton");
    ui->changeButton->setObjectName("changeButton");
    ui->titleLabel->setObjectName("titleLabel");
    ui->stackedWidget->setObjectName("stackedWidget");

    line = new QFrame(this);
    line->setStyleSheet("background-color: #ffd900;");
    line->setFixedSize(35, 3);
    ui->verticalLayout_3->insertWidget(2, line, 0, Qt::AlignCenter);
    line->setContentsMargins(0, 5, 0, 0); // сверху 10px отступ


    validators();
    applyStyleSheetDialog();
}

RegistrDialog::~RegistrDialog()
{
    delete ui;
}

void RegistrDialog::on_acceptButton_clicked()
{
    if(currentButton) {
        if(emptyLineEdit(ui->login_page)) {
            return;
        }
    } else {
        if(emptyLineEdit(ui->create_page)) {
            return;
        }
    }

    if(currentButton) {
        signIn();
    } else {
        createAccount();
    }
}

void RegistrDialog::signIn()
{
    QString email = ui->lineEditEmail_1->text();
    QString password = ui->lineEditPassword_1->text();

    QSqlQuery query;
    query.prepare("SELECT * FROM \"Users\" WHERE email = :email AND password = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if(query.exec()) {
        if(query.next()) {
            currentUser.id_user = query.value("id_user").toInt();
            currentUser.name = query.value("name").toString();
            currentUser.surname = query.value("surname").toString();
            currentUser.email = query.value("email").toString();
            currentUser.phone = query.value("phone").toString();

            parentWindow->setData(currentUser);

            this->close();
        } else {
            ui->incorrectData->setText("Incorrect email or password");
            ui->incorrectData->setStyleSheet("color: red;");
            ui->lineEditEmail_1->setStyleSheet("border: 1px solid red;");
            ui->lineEditPassword_1->setStyleSheet("border: 1px solid red;");
        }
    }
}

void RegistrDialog::createAccount()
{
    QSqlQuery query;

    QString name = ui->lineEditName->text();
    QString surname = ui->lineEditSurname->text();
    QString email = ui->lineEditEmail_2->text();
    QString phoneNumber = ui->lineEditNumber->text();
    QString password = ui->lineEditPassword_2->text();

    query.prepare("SELECT \"email\" FROM \"Users\" WHERE email = :email");
    query.bindValue(":email", email);

    ui->incorrectEmail->setText("");
    if(query.exec() && query.next()) {
        ui->incorrectEmail->setText("Email already exists");
        ui->incorrectEmail->setStyleSheet("color: red;");
        ui->lineEditEmail_2->setStyleSheet("border: 1px solid red;");
    } else if(!isRightPassword(password)) {
        ui->exclamationMark->setPixmap(QPixmap("C:/Users/golov/Downloads/exclamationmark_red.png"));
        ui->lineEditPassword_2->setStyleSheet("border: 1px solid red;");
    } else {
        query.prepare("INSERT INTO \"Users\" (\"name\", \"surname\", \"email\", \"password\", \"phone\")"
                      "VALUES(:name, :surname, :email, :password, :phone)");

        query.bindValue(":name", name);
        query.bindValue(":surname", surname);
        query.bindValue(":email", email);
        query.bindValue(":phone", phoneNumber);
        query.bindValue(":password", password);

        if(query.exec()) {
            currentUser.id_user = query.value("id").toInt();
            currentUser.name = query.value("name").toString();
            currentUser.surname = query.value("surname").toString();
            currentUser.email = query.value("email").toString();
            currentUser.phone = query.value("phone").toString();
            currentUser.password = query.value("phone").toString();

            this->close();
        }
    }
}

bool RegistrDialog::emptyLineEdit(QWidget* parent)
{
    bool hasEmptyFields = false;

    for (QLineEdit* lineEdit : parent->findChildren<QLineEdit*>()) {
        if (lineEdit->text().isEmpty()) {
            lineEdit->setStyleSheet("border: 1px solid red;");
            lineEdit->setPlaceholderText("The field can't be blank");
            hasEmptyFields = true;
        } else {
            lineEdit->setStyleSheet("");
        }
    }
    return hasEmptyFields;
}

bool RegistrDialog::isRightPassword(QString password)
{
    QRegularExpression passwordExpr("^(?=.*[A-Z])(?=.*[a-z])(?=.*[0-9])(?=.*[!@#$%^&*_\\)\\(]).{8,30}$");
    QRegularExpressionValidator validator(passwordExpr, this);

    int pos = 0;
    if (validator.validate(const_cast<QString&>(password), pos) == QValidator::Acceptable) {
        ui->incorrectPassword->setText("");
        return true;
    }

    ui->incorrectPassword->setText("The password is incorrect");
    ui->incorrectPassword->setStyleSheet("color: red;");

    return false;
}


void RegistrDialog::on_changeButton_clicked()
{
    if(currentButton) {
        ui->acceptButton->setText("CREATE");
        ui->changeButton->setText("SIGN IN ACCOUNT");
        ui->stackedWidget->setCurrentWidget(ui->create_page);
        ui->titleLabel->setText("Create Account");
    } else {
        ui->acceptButton->setText("SIGN IN");
        ui->changeButton->setText("CREATE ACCOUNT");
        ui->stackedWidget->setCurrentWidget(ui->login_page);
        ui->titleLabel->setText("Login");
    }
    ui->exclamationMark->setPixmap(QPixmap("C:/Users/golov/Downloads/exclamationmark_white.png"));
    currentButton = !currentButton;

    for(QLineEdit *lineEdit : ui->stackedWidget->findChildren<QLineEdit*>()) {
        lineEdit->setText("");
        lineEdit->setStyleSheet("");
        lineEdit->setPlaceholderText("");
    }
}


void RegistrDialog::applyStyleSheetDialog()
{
    QFile file(":/registrDialog.css");
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
}

void RegistrDialog::validators()
{
    QRegularExpression NameExpr("^[A-Za-zА-Яа-яЁё]{2,15}$");
    QRegularExpressionValidator *NameValidator = new QRegularExpressionValidator(NameExpr, this);
    ui->lineEditName->setValidator(NameValidator);

    QRegularExpression SurnameExpr("^[A-Za-zА-Яа-яЁё]{2,15}$");
    QRegularExpressionValidator *SurnameValidator = new QRegularExpressionValidator(SurnameExpr, this);
    ui->lineEditSurname->setValidator(SurnameValidator);

    QRegularExpression PhoneExpr("^\\+?[0-9]{7,16}$");
    QRegularExpressionValidator *PhoneValidator = new QRegularExpressionValidator(PhoneExpr, this);
    ui->lineEditNumber->setValidator(PhoneValidator);
}
