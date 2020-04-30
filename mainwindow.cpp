#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //  Initialize Database
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Question,
                                        "Mini kolokwium", "Czy to Ania?", QMessageBox::Yes|QMessageBox::No).exec())
    {
        this->initDB(true); // Ania's password
    } else {
        this->initDB(false); // Filip's password
    }

    ui->tabWidget->addTab(new QTableView(),"Smieciarka");
    ui->tabWidget->addTab(new QTableView(),"Odpad");
    ui->tabWidget->addTab(new QTableView(),"Material");
    ui->tabWidget->addTab(new QTableView(),"Kontener");
    ui->tabWidget->addTab(new QTableView(),"Firma");
    ui->tabWidget->addTab(new QTableView(),"Sprzedaze");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Connects with DB server and prints error text on qDebug if anything's wrong */
void MainWindow::initDB(bool ItsAnia) {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    if(ItsAnia){
        db.setPassword("bernas1998!");
    } else {
        db.setPassword("Komputer1@");
    }
    db.setDatabaseName("mydb");
    if(!db.open()){
        qDebug() << "Cos nie pyklo :( " << db.lastError().text();
    }
}

QSqlTableModel *MainWindow::initModel(const char* TableName){
    QSqlTableModel* model = new QSqlTableModel(this, db);
    model->setTable(TableName);
    //All changes to the model will be applied immediately to the database:
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select(); // Reads data from Table
    std::string strTn = std::string(TableName);
    qDebug() << strTn.c_str();
    if (strTn =="Smieciarka" )
    {
        qDebug() <<"wchodzi";
        model->setHeaderData(0, Qt::Horizontal, "Rejestracja");
        model->setHeaderData(1, Qt::Horizontal, "Waga");
        model->setHeaderData(2, Qt::Horizontal, "Data odbioru");
    }
    else if(strTn == "Odpad")
    {
        model->setHeaderData(0, Qt::Horizontal, "Id");
        model->setHeaderData(1, Qt::Horizontal, "Zgodnosc [%]");
        model->setHeaderData(2, Qt::Horizontal, "Smieciarka");
        model->setHeaderData(3, Qt::Horizontal, "Rejestracja");
        model->setHeaderData(4, Qt::Horizontal, "Kontener");
    }
    else if(strTn == "Material")
    {
        model->setHeaderData(0, Qt::Horizontal, "Nazwa");
        model->setHeaderData(1, Qt::Horizontal, "Min zgodnosc [%]");
    }
    else if(strTn == "Kontener")
    {
        model->setHeaderData(0, Qt::Horizontal, "Id");
        model->setHeaderData(1, Qt::Horizontal, "Waga");
        model->setHeaderData(2, Qt::Horizontal, "Material");
    }
    else if(strTn == "Firma")
    {
        model->setHeaderData(0, Qt::Horizontal, "Nazwa");
        model->setHeaderData(1, Qt::Horizontal, "Cena za kg");
        model->setHeaderData(2, Qt::Horizontal, "Material");
    }
    else if(strTn == "Sprzedaze")
    {
        model->setHeaderData(0, Qt::Horizontal, "Id");
        model->setHeaderData(1, Qt::Horizontal, "Data");
        model->setHeaderData(2, Qt::Horizontal, "Firma");
        model->setHeaderData(3, Qt::Horizontal, "Kontener");
    }
    else
    {
        qDebug() << "wrong TableName";
    }

    // do ustawiania naglowkow kolumn(inaczej uzywa meh nazw z bazy)
    //    model->setHeaderData(0, Qt::Horizontal, "Item Name");
    //    model->setHeaderData(1, Qt::Horizontal, "Price");
    return model;
}



//przy zmiane karty view przechowuje QTableView znajdujące sie na aktualnej karcie
//model jest ustawiany na null żeby tablica nie wyświetlała sie odrazu
void MainWindow::on_tabWidget_currentChanged(int index)
{
    view=dynamic_cast<QTableView*>(ui->tabWidget->widget(index));
    view->setEditTriggers(QTableView::NoEditTriggers);

    //view->setModel(nullptr);
}

// po kliknięciu przycisku pojawia sie tabela w zależności od aktualnej karty
void MainWindow::on_pushButtonPokaAll_clicked()
{
    QString currentTabName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    view->setModel(initModel(currentTabName.toStdString().c_str()));
}
