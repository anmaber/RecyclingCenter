#include "mainwindow.h"
//#include "database.hh"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Halo halo, tescik

    return a.exec();
}
