#include "dialog.h"
#include "ui_dialog.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);    

    //Connect statements for Algorithm Testing
    ui->sourceBox->setRange(0,17);
    ui->destBox->setRange(0,17);
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(dij_mainprogram()));
    connect(ui->clearButton,SIGNAL(clicked()),this,SLOT(RESETVALUE()));
    connect(ui->clearButton,SIGNAL(clicked()),ui->distanceEdit,SLOT(clear()));
    connect(ui->clearButton,SIGNAL(clicked()),ui->pathEdit,SLOT(clear()));

    //Connect statements for SQL Testing
    connect(ui->train_infoButton,SIGNAL(clicked()),this,SLOT(train_info()));
    connect(ui->switch_infoButton,SIGNAL(clicked()),this,SLOT(switch_info()));
    connect(ui->path_infoButton,SIGNAL(clicked()),this,SLOT(path_info()));
    connect(ui->throttle_infoButton,SIGNAL(clicked()),this,SLOT(throttle_info()));
    connect(ui->queryButton,SIGNAL(clicked()),this,SLOT(sql_query()));
    connect(ui->createtablesButton,SIGNAL(clicked()),this,SLOT(create_sqltables()));
    connect(ui->server_connectButton,SIGNAL(clicked()),this,SLOT(sqlserver_connect()));

}

void Dialog::sql_query()
{

}

QTableView *Dialog::createView(QSqlTableModel *model, const QString &title)
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle(title);
    view->show();
    return view;
}


Dialog::~Dialog()
{
    delete ui;
}

void Dialog::RESETVALUE()
{
    ui->sourceBox->setValue(0);
    ui->destBox->setValue(0);
}

void Dialog::train_info()
{
    model = new QSqlQueryModel;
    model->setQuery(k);
    view = new QTableView;
    view->setModel(model);
    view->show();
}

void Dialog::switch_info()
{
    model = new QSqlQueryModel;
    model->setQuery(q);
    view = new QTableView;
    view->setModel(model);
    view->show();
}

void Dialog::path_info()
{
    model = new QSqlQueryModel;
    model->setQuery(l);
    view = new QTableView;
    view->setModel(model);
    view->show();
}

void Dialog::throttle_info()
{
    model = new QSqlQueryModel;
    model->setQuery(j);
    view = new QTableView;
    view->setModel(model);
    view->show();
}

void Dialog::sqlserver_connect()
{

}

void Dialog::create_sqltables()
{
    //Connect to SQL Server
    //QSqlTableModel model;
    db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setHostName("localhost");
    db.setDatabaseName(":memory:");
    if (!db.open())
    {
        qDebug() << "Error";
    }

    db.open();

    QString query = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = 'title'";
    q = db.exec(query);
    k = db.exec(query);
    l = db.exec(query);
    j = db.exec(query);

    //Create Switch_Info Table
     q.exec("CREATE TABLE switchInfoTable (switch TEXT, position TEXT);");
     q.exec("INSERT INTO switchInfoTable VALUES ('1', 'Open');");
     q.exec("INSERT INTO switchInfoTable VALUES ('2', 'Open');");
     q.exec("INSERT INTO switchInfoTable VALUES ('3', 'Open');");
     q.exec("INSERT INTO switchInfoTable VALUES ('4', 'Open');");
     q.exec("SELECT * FROM switchInfoTable;");

    //Create Throttle_Info Table
     j.exec("CREATE TABLE throttleInfoTable (train TEXT, direction TEXT, throttle INT);");
     j.exec("INSERT INTO throttleInfoTable (train, direction, throttle) VALUES ('Georgia Express', Forward', 7);");
     j.exec("INSERT INTO throttleInfoTable (train, direction, throttle) VALUES ('Alabama Express', 'Forward', 1);");
     j.exec("SELECT * FROM throttleInfoTable;");

    //Create Train_Info Table
     k.exec("CREATE TABLE trainInfoTable (train TEXT, currentOcc TEXT, destination TEXT, next TEXT, path INT);");
     k.exec("INSERT INTO trainInfoTable (train, currentOcc, destination, next, path) VALUES ('Alabama Express', 'Full', 'Chicago', 'Denver', 1);");
     k.exec("INSERT INTO trainInfoTable (train, currentOcc, destination, next, path) VALUES ('Georgia Express', 'Full', 'Los Angeles', 'Denver', 2);");
     k.exec("SELECT * FROM trainInfoTable;");

    //Create Path_Info Table
     l.exec("CREATE TABLE pathInfoTable (pathId INT, next2 TEXT, next11 TEXT);");
     l.exec("INSERT INTO pathInfoTable (pathId, next2, next11) VALUES (1, '3 5 9 11 31 37 50 52 44 48', '1 2 7 18 20 12 14 17 6 34');");
     l.exec("INSERT INTO pathInfoTable (pathId, next2, next11) VALUES (2, '1 7 10 8 21 27 16 19 13 9', '32 35 41 44 45 87 89 76 90 91');");
     l.exec("SELECT * FROM pathInfoTable;");

     ui->createtablesButton->setDisabled(1);
     ui->createtablesButton->setText("Tables now exists!");
}
