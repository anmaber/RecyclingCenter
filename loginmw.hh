#ifndef LOGINMW_HH
#define LOGINMW_HH

#include <QMainWindow>
#include <QString>
#include <QSqlTableModel>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>

#include "mainwindow.h"

namespace Ui {
class LoginMW;
}

class LoginMW : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginMW(QWidget *parent = nullptr);
    ~LoginMW();

private slots:
    void on_pushButton_Login_clicked();

    void on_buttonBox_Admin_accepted();

    void on_buttonBox_Admin_rejected();

private:
    Ui::LoginMW *ui;
    QSqlDatabase db;
    MainWindow *mw;
};

#endif // LOGINMW_HH
