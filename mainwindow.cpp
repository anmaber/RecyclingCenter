#include "mainwindow.h"
#include "ui_mainwindow.h"

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

QSqlTableModel *MainWindow::initModel(const QString & TableName){
    QSqlTableModel* model = new QSqlTableModel(this, db);
    model->setTable(TableName);
    //All changes to the model will be applied immediately to the database:
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select(); // Reads data from Table

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
    view->setModel(nullptr);
}

// po kliknięciu przycisku pojawia sie tabela w zależności od aktualnej karty
void MainWindow::on_pushButtonPokaAll_clicked()
{
    QString currentTabName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    view->setModel(initModel(currentTabName));
}
