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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_tabWidget_currentChanged(int index);

    void on_pushButtonPokaAll_clicked();

    void on_pushButtonUsun_clicked();

    void on_textEdit_textChanged();

    void on_pushButtonDodaj_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    void initDB(bool ItsAnia);
    QTableView * view;
    QSqlTableModel *initModel(const char* TableName); // reads Table from server

};

#endif // MAINWINDOW_H
