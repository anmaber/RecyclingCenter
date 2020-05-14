#include "loginwindow.hh"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::on_buttonBox_accepted() {
    // Laczenie Qstringow
    QString InputData;

    InputData = ui->lineEdit_Nazwa->text() + " " + ui->lineEdit_Haslo->text();

    emit LoginData(InputData);
}
