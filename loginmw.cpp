#include "loginmw.hh"
#include "ui_loginmw.h"

LoginMW::LoginMW(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginMW)
{
    ui->setupUi(this);

    // Basic setup of connection. Has to be done only once(not in Login signal).
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("mydb");
}

LoginMW::~LoginMW()
{
    delete ui;
}

void LoginMW::CleanLineEdit()
{
    ui->lineEdit_Nazwa->clear();
    ui->lineEdit_Haslo->clear();
    ui->label_Status->setText("Poprawnie wylogowano");
}

void LoginMW::on_pushButton_Login_clicked()
{
    // Read Username and Password from LineEdits
    QString UserName = ui->lineEdit_Nazwa->text();
    db.setUserName(UserName);
    db.setPassword(ui->lineEdit_Haslo->text());

    if(db.open()){
        ui->label_Status->setText("Poprawnie zalogowano");
        this->hide();
        mw = new MainWindow(UserName, db, this); // Init MainWindow with correct DB connection
        mw->show();
    } else {
        ui->label_Status->setText("Zla nazwa lub haslo");
        qDebug("Cos nie pyklo:");
        qDebug() << db.lastError().text();
    }
}
