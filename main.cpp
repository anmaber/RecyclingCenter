#include "mainwindow.h"
#include "loginmw.hh"
#include <QApplication>
#include <QMessageBox>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginMW lmw;

    lmw.show();

    return a.exec();
}
