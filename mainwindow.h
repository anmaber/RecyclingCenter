#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString UserName, QSqlDatabase ParentDB, QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_tabWidget_currentChanged(int index);

    void on_pushButtonPokaAll_clicked();

    void on_pushButtonUsun_clicked();

    void on_textEdit_textChanged();

    void on_pushButtonDodaj_clicked();

    void on_pushButtonZatwierdz_clicked();

    void on_pushButtonWyloguj_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QTableView * view;
    QSqlRelationalTableModel *initModel(const char* TableName); // reads Table from server

};

#endif // MAINWINDOW_H
