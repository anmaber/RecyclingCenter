#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    void initDB(bool ItsAnia);
    QSqlTableModel *initModel(const char*TableName); // reads Table from server

};

#endif // MAINWINDOW_H
