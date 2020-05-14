#include "loginmw.hh"
#include "ui_loginmw.h"

LoginMW::LoginMW(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginMW)
{
    ui->setupUi(this);
}

LoginMW::~LoginMW()
{
    delete ui;
}
