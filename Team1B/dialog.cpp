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
    connect(ui->trottle_infoButton,SIGNAL(clicked()),this,SLOT(trottle_info()));
    connect(ui->queryButton,SIGNAL(clicked()),this,SLOT(sql_query()));
    connect(ui->createtablesButton,SIGNAL(clicked()),this,SLOT(create_sqltables()));

    //Connect to SQL Server
    //QSqlTableModel model;
    QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setHostName("localhost");
    db.setDatabaseName(":memory:");
    if (!db.open())
    {
        qDebug() << "Error";
    }

    db.open();
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

}

void Dialog::switch_info()
{

}

void Dialog::path_info()
{

}

void Dialog::trottle_info()
{

}

void Dialog::sql_query()
{

}

void Dialog::create_sqltables()
{

}
