#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QDate>

#include "loginmw.hh"

MainWindow::MainWindow(QString UserName, QSqlDatabase ParentDB, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Init Database from parent's connection
    db = ParentDB;

    if(UserName == "root"){
        ui->tabWidget->addTab(new QTableView(),"Smieciarka");
        ui->tabWidget->addTab(new QTableView(),"Odpad");
        ui->tabWidget->addTab(new QTableView(),"Material");
        ui->tabWidget->addTab(new QTableView(),"Kontener");
        ui->tabWidget->addTab(new QTableView(),"Firma");
        ui->tabWidget->addTab(new QTableView(),"Sprzedaze");
    } else if(UserName == "inz") {
        ui->tabWidget->addTab(new QTableView(),"Odpad");
        ui->tabWidget->addTab(new QTableView(),"Material");
    } else if(UserName == "handel") {
        ui->tabWidget->addTab(new QTableView(),"Kontener");
        ui->tabWidget->addTab(new QTableView(),"Firma");
        ui->tabWidget->addTab(new QTableView(),"Sprzedaze");
    } else if(UserName == "oper"){
        ui->tabWidget->addTab(new QTableView(),"Smieciarka");
        ui->tabWidget->addTab(new QTableView(),"Odpad");
        ui->tabWidget->addTab(new QTableView(),"Kontener");
    } else {
        qDebug("Niepoprawna nazwa uzytkownika");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

QSqlRelationalTableModel *MainWindow::initModel(const char* TableName){
    QSqlRelationalTableModel* model = new QSqlRelationalTableModel(this, db);
    model->setTable(TableName);
    //All changes to the model will be applied immediately to the database:
    model->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
//    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);

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
        //Relations

        model->setRelation(2, QSqlRelation("Smieciarka","Nr_Rejestracyjny","Nr_Rejestracyjny"));
        model->setRelation(3, QSqlRelation("Material","Nazwa_Mat","Nazwa_Mat"));
//        model->setRelation(2, QSqlRelation("Smieciarka","Nr_rejestracyjny","Nr_rejestracyjny"));
//        model->setRelation(3, QSqlRelation("Material","Nazwa","Nazwa"));

        model->setRelation(4, QSqlRelation("Kontener","idKontener","idKontener"));
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
        //Relations

        model->setRelation(2, QSqlRelation("Material","Nazwa_Mat","Nazwa_Mat"));
//        model->setRelation(2, QSqlRelation("Material","Nazwa","Nazwa"));

    }
    else if(strTn == "Firma")
    {
        model->setHeaderData(0, Qt::Horizontal, "Nazwa");
        model->setHeaderData(1, Qt::Horizontal, "Cena za kg");
        model->setHeaderData(2, Qt::Horizontal, "Material");
        //Relations
        model->setRelation(2, QSqlRelation("Material","Nazwa_Mat","Nazwa_Mat"));
//        model->setRelation(2, QSqlRelation("Material","Nazwa","Nazwa"));

    }
    else if(strTn == "Sprzedaze")
    {
        model->setHeaderData(0, Qt::Horizontal, "Id");
        model->setHeaderData(1, Qt::Horizontal, "Data");
        model->setHeaderData(2, Qt::Horizontal, "Firma");
        model->setHeaderData(3, Qt::Horizontal, "Kontener");
        //Relations
        model->setRelation(2, QSqlRelation("Firma","Nazwa_Firmy","Nazwa_Firmy"));
//        model->setRelation(2, QSqlRelation("Firma","Nazwa","Nazwa"));
        model->setRelation(3, QSqlRelation("Kontener","idKontener","idKontener"));
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
    view->setEditTriggers(QTableView::DoubleClicked);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

// po kliknięciu przycisku pojawia sie tabela w zależności od aktualnej karty
void MainWindow::on_pushButtonPokaAll_clicked()
{
    QString currentTabName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    view->setModel(initModel(currentTabName.toStdString().c_str()));
    view->setItemDelegate(new QSqlRelationalDelegate(view));
}

//          Removing
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
            // refresh view by clicking Zatwierdz or Pokaz wszystko

            //odświezenie zeby nie był pusty rzad, tez mi sie nie podoba ten dynamic_cast
//            auto modelToUpdate = dynamic_cast<QSqlRelationalTableModel*>(view->model());
//            modelToUpdate->select();

            /* Znalazłem, ogolnie info z tym (!):
            Likewise, if you remove rows using removeRows(),
            the rows will be marked with an exclamation mark (!) until the change is submitted.
            https://www3.sra.co.jp/qt/relation/doc/qtsql/sql-presenting.html
            Ale to nie pomaga: :(
            modelToUpdate->submitAll();
            */

        }
    } else {
        qDebug() << "Zaznacz jakiś widok mordo";
    }
}

//szukanie, rozwala sie jak chcesz wyszukać gdy nic nie jest pokazane, to sie samo ogarnie jak wywali sie pokaz wszystkie

// Odpad i Firma nie dziala

void MainWindow::on_textEdit_textChanged()
{
    auto modelToSearch = dynamic_cast<QSqlRelationalTableModel*>(view->model());
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

//          Inserting/Adding
void MainWindow::on_pushButtonDodaj_clicked() {
    // Wersja min. to (analogicznie jak usuwanie)
    // view->model()->insertRow(view->model()->rowCount());
    // Jednak uzywajac insertRecord, można ustawiać auto dane.

    auto CurrentModel = dynamic_cast<QSqlRelationalTableModel*>(view->model());
    QSqlRecord newRecord = CurrentModel->record();
    // ManualSubmit required coz of lack of default values, i.e.:
    // "Field 'Material_Nazwa' doesn't have a default value QMYSQL3: Unable to execute statement"
    CurrentModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);

    // Extract currentTableName as a std::string
    QString currentTabName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    std::string strTn = std::string(currentTabName.toStdString().c_str());

    // Regarding Autoincrement ID:
    // There shouldn't be need to declare it, i.e:
    // newRecord.remove(newRecord.indexOf("idKontener"));
    // but as it's not working, I'm using a workaround:
    // newRecord.setValue(0, view->model()->rowCount()+1);
    // ... but there is a risk of recurring ID -> maybe need to TRIGGER correction

    // Different auto data, depending on the Table
    if (strTn == "Smieciarka" ){
        newRecord.setValue(2, QDate::currentDate());
    } else if(strTn == "Odpad") {
        newRecord.setValue(0, view->model()->rowCount()+1);
    } else if(strTn == "Kontener") {
        newRecord.setValue(0, view->model()->rowCount()+1);
    } else if(strTn == "Sprzedaze") {
        newRecord.setValue(0, view->model()->rowCount()+1);
        newRecord.setValue(1, QDate::currentDate());
    }

    // Append newRecord to CurrentModel
    if(CurrentModel->insertRecord(-1, newRecord)){
       CurrentModel->submitAll(); // Submit new Row, and maybe some auto data
    } else {
        db.rollback(); // cancels last transition
        qDebug() << " Insertion error: " << CurrentModel->lastError().text();
    }
    // Doesn't work here, have to be outside function(I've kept it as a back up):
    // CurrentModel->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
}

// Zatwierdzanie Dodawania lub odswiezanie widoku po usuwaniu(#FakeRemove)
void MainWindow::on_pushButtonZatwierdz_clicked()
{
    if(view){
        auto CurrentModel = dynamic_cast<QSqlRelationalTableModel*>(view->model());
        CurrentModel->submitAll(); // Submit user input changes
        CurrentModel->select(); // A bow to removeRow()
        // Have to change Strategy after adding record, dunno why :(
        CurrentModel->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
    } else {
        qDebug() << "No view, no fun";
    }
}

void MainWindow::on_pushButtonWyloguj_clicked()
{
//    parent->show();
//    (QWidget*)parent->show();

//    auto LoginWindow = qobject_cast<QWidget*>(this->parent());
    auto LoginWindow = qobject_cast<LoginMW*>(this->parent());
    LoginWindow->CleanLineEdit();
    LoginWindow->show();

    this->close();
}
