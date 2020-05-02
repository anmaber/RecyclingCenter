#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QDate>

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
//    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select(); // Reads data from Table
    std::string strTn = std::string(TableName);

    if (strTn =="Smieciarka" )
    {
        model->setHeaderData(0, Qt::Horizontal, "Rejestracja");
        model->setHeaderData(1, Qt::Horizontal, "Waga");
        model->setHeaderData(2, Qt::Horizontal, "Data-odbioru");
    }
    else if(strTn == "Odpad")
    {
        model->setHeaderData(0, Qt::Horizontal, "Id");
        model->setHeaderData(1, Qt::Horizontal, "Zgodnosc [%]");
        model->setHeaderData(2, Qt::Horizontal, "Smieciarka");
        model->setHeaderData(3, Qt::Horizontal, "Material");
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

    return model;
}

//przy zmiane karty view przechowuje QTableView znajdujące sie na aktualnej karcie
//po podwójnym kliknięciu możne edytować komórke
//przy kliknięciu zaznacza sie odrazu cały rząd
//przytrzymując ctrl można zaznaczyc wiecej rzędów
void MainWindow::on_tabWidget_currentChanged(int index)
{
    view=dynamic_cast<QTableView*>(ui->tabWidget->widget(index));
    // PRZY EDIT FK DOBRZE BY BYŁO DAĆ COMBO BOX I ERROR HANDLING
    view->setEditTriggers(QTableView::DoubleClicked);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

// po kliknięciu przycisku pojawia sie tabela w zależności od aktualnej karty
void MainWindow::on_pushButtonPokaAll_clicked()
{
    QString currentTabName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    view->setModel(initModel(currentTabName.toStdString().c_str()));
}

//usuwanie
void MainWindow::on_pushButtonUsun_clicked()
{
    if(view)
    {
        QItemSelectionModel *select = view->selectionModel();
        if(select->hasSelection())
        {
            auto selected = select->selectedRows();
            for(auto elem : selected)
            {
                view->model()->removeRow(elem.row());
            }
            //odświezenie zeby nie był pusty rzad, tez mi sie nie podoba ten dynamic_cast
            auto modelToUpdate = dynamic_cast<QSqlTableModel*>(view->model());
            modelToUpdate->select();
            /* Znalazłem, ogolnie info:
            Likewise, if you remove rows using removeRows(),
            the rows will be marked with an exclamation mark (!) until the change is submitted.
            https://www3.sra.co.jp/qt/relation/doc/qtsql/sql-presenting.html

            Ale to nie pomaga: :(
//            modelToUpdate->submitAll();
//            view->model()->submitAll();
            */

        }
    }
}

//szukanie, rozwala sie jak chcesz wyszukać gdy nic nie jest pokazane, to sie samo ogarnie jak wywali sie pokaz wszystkie
void MainWindow::on_textEdit_textChanged()
{
    auto modelToSearch = dynamic_cast<QSqlTableModel*>(view->model());
    int columns = modelToSearch->columnCount();
    //filtrowanie jest po prostu zapytaniem WHERE z SQL
    //modelToSearch->record().fieldName uzyskuje nazwe kolumny taka jaka jest w bazie
    //cos typu "* = coś" nie zadziala, musimy znac nazwy kolumn i ich ilosc zeby przeszukac wszystkie
    //zapytanie typu " kolumna like 'cos%' " zapewnia nam wyszukiwanie zawartosći zaczynajacej sie na 'cos'
    QString filter = modelToSearch->record().fieldName(0) + " like '" + ui->textEdit->toPlainText() + "%'";
    for(int i = 1; i< columns ;++i)
    {
        filter+=" or ";
        filter += modelToSearch->record().fieldName(i) + " like '" + ui->textEdit->toPlainText() + "%'";
    }
    modelToSearch->setFilter(filter);
    qDebug()<<filter;

}

void MainWindow::on_pushButtonDodaj_clicked()
{
    // Extract currentTableName as a std::string
    QString currentTabName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    std::string strTn = std::string(currentTabName.toStdString().c_str());

    auto CurrentModel = dynamic_cast<QSqlTableModel*>(view->model());

    QSqlRecord newRecord = CurrentModel->record();

//    int columns = CurrentModel->columnCount();
//    for (int i = 0; i < columns; ++i) {
//        newRecord.setNull(i);
//    }


    if (strTn == "Smieciarka" ){
        newRecord.setValue(0, "XXXXXXX");
        newRecord.setValue(1, 1000);
        qDebug() << QDate::currentDate();
        newRecord.setValue(2, QDate::currentDate());
    } else if(strTn == "Odpad") {
        // idOdpad is autoincrement so no need to declare it
        newRecord.remove(newRecord.indexOf("idOdpad"));
        newRecord.setValue("Procent_zgodnosci", 0);
//        newRecord.setNull("Smieciarka_Nr_rejestracyjny");
//        newRecord.setNull("Material_Nazwa");
        newRecord.setValue("Smieciarka_Nr_rejestracyjny", "ssss");
        newRecord.setValue("Material_Nazwa", "ssss");

        newRecord.setValue("Kontener_idKontener", 1);
    } else if(strTn == "Material") {
        newRecord.setValue(0, "XXXXXXX");
        newRecord.setValue(1, 90);
    } else if(strTn == "Kontener") {

    } else if(strTn == "Firma") {

    } else if(strTn == "Sprzedaze") {

    } else {
        qDebug() << "dodawanie nie pyklo";
    }

    if(CurrentModel->insertRecord(-1, newRecord)){
       qDebug()<<"successful insertion";
       CurrentModel->submitAll();
//       CurrentModel->select();
    } else {
        qDebug() << "insertion error: " << CurrentModel->lastError().text();
        db.rollback(); // cancels last transition
    }
}
