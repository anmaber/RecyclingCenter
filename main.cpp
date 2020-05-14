#include "mainwindow.h"
#include "loginwindow.hh"
#include <QApplication>
#include <QMessageBox>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
    LoginWindow l;

//    w.show();

                                         // create login-widget
//    QObject::connect(&l, SIGNAL(LoginWindow::loggedIn), &w, SLOT(MainWindow::show()));    // connect to mainwindow show()
//    l.show();                                          // show login-window instead of
//                                                       // mainwindow at start

    if (l.exec() == QDialog::Accepted) {
//        db = l.ZwrocPolaczenieDB();
//        w.PrzekazPolaczenieDB(db);
        w.show();
    }

    return a.exec();
}
