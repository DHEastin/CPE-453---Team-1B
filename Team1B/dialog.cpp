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

    this->setWindowTitle("Testing_Framework");

    //Connect statements for Algorithm Testing
    ui->sourceBox->setRange(0,17);
    ui->destBox->setRange(0,17);
    connect(ui->swapButton,SIGNAL(clicked()),this,SLOT(swapNUM()));
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(dij_mainprogram()));
    connect(ui->clearButton,SIGNAL(clicked()),this,SLOT(RESETVALUE()));
    connect(ui->clearButton,SIGNAL(clicked()),ui->distanceEdit,SLOT(clear()));
    connect(ui->clearButton,SIGNAL(clicked()),ui->pathEdit,SLOT(clear()));

    //Connect statements for SQL Testing
    connect(ui->train_infoButton,SIGNAL(clicked()),this,SLOT(train_info()));
    connect(ui->switch_infoButton,SIGNAL(clicked()),this,SLOT(switch_info()));
    connect(ui->path_infoButton,SIGNAL(clicked()),this,SLOT(path_info()));
    connect(ui->track_infoButton,SIGNAL(clicked()),this,SLOT(track_info()));
    connect(ui->track_listingButton,SIGNAL(clicked()),this,SLOT(track_listing()));
    connect(ui->throttle_infoButton,SIGNAL(clicked()),this,SLOT(throttle_info()));
    connect(ui->queryButton,SIGNAL(clicked()),this,SLOT(sql_query()));
    connect(ui->createtablesButton,SIGNAL(clicked()),this,SLOT(create_sqltables()));
    connect(ui->server_connectButton,SIGNAL(clicked()),this,SLOT(sqlserver_connect()));

}

void Dialog::sql_query()
{

}

void Dialog::swapNUM()
{
    int BEG = ui->sourceBox->value();
    int END = ui->destBox->value();
    int swap = BEG;
    ui->sourceBox->setValue(END);
    ui->destBox->setValue(swap);
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

void Dialog::track_info()
{
    model = new QSqlQueryModel;
    model->setQuery(m);
    view = new QTableView;
    view->setModel(model);
    view->show();
}

void Dialog::track_listing()
{
    model = new QSqlQueryModel;
    model->setQuery(n);
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
    m = db.exec(query);
    n = db.exec(query);

    //Create Switch_Info Table
    //Table list switches and Open/Closed status
    //Unused but code was created to read from this table and do nothing with it
     q.exec("CREATE TABLE switchInfoTable (switch TEXT, position TEXT);");
     q.exec("INSERT INTO switchInfoTable VALUES ('SW1', 'Open');");
     q.exec("INSERT INTO switchInfoTable VALUES ('SW2', 'Open');");
     q.exec("INSERT INTO switchInfoTable VALUES ('SW3', 'Open');");
     q.exec("INSERT INTO switchInfoTable VALUES ('SW4', 'Open');");
     q.exec("SELECT * FROM switchInfoTable;");

    //Create Throttle_Info Table
    //CURRENTLY NOT IMPORTANT
     j.exec("CREATE TABLE throttleInfoTable (train TEXT, direction TEXT, throttle INT);");
     j.exec("INSERT INTO throttleInfoTable (train, direction, throttle) VALUES ('Georgia Express', Forward', 7);");
     j.exec("INSERT INTO throttleInfoTable (train, direction, throttle) VALUES ('Alabama Express', 'Forward', 1);");
     j.exec("SELECT * FROM throttleInfoTable;");

    //Create Train_Info Table
    //CURRENTLY NOT IMPORTANT
     k.exec("CREATE TABLE trainInfoTable (train TEXT, currentOcc TEXT, destination TEXT, next TEXT, path INT);");
     k.exec("INSERT INTO trainInfoTable (train, currentOcc, destination, next, path) VALUES ('Alabama Express', 'Full', 'Chicago', 'Denver', 1);");
     k.exec("INSERT INTO trainInfoTable (train, currentOcc, destination, next, path) VALUES ('Georgia Express', 'Full', 'Los Angeles', 'Denver', 2);");
     k.exec("SELECT * FROM trainInfoTable;");

    //Create Path_Info Table
    //CURRENTLY NOT IMPORTANT
     l.exec("CREATE TABLE pathInfoTable (pathId INT, next2 TEXT, next11 TEXT);");
     l.exec("INSERT INTO pathInfoTable (pathId, next2, next11) VALUES (1, '3 5 9 11 31 37 50 52 44 48', '1 2 7 18 20 12 14 17 6 34');");
     l.exec("INSERT INTO pathInfoTable (pathId, next2, next11) VALUES (2, '1 7 10 8 21 27 16 19 13 9', '32 35 41 44 45 87 89 76 90 91');");
     l.exec("SELECT * FROM pathInfoTable;");

     //Create Track_Info Table
     //Table shows how nodes are connected.
     //When reading from this, code was created to create the double sided connection
      m.exec("CREATE TABLE trackInfoTable (currentnode INT, nextnode INT, weight INT);");
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (0,2,33);");//Left Side Middle Connect
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (0,3,33);");//Left Side Middle Connect
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (2,4,5);");//Switch 1 Bypass
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (3,4,5);");//Switch 1 Bypass
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (4,5,5);");//SW1 & SW2 Middle
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (5,6,5);");//SW1 & SW2 Middle
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (6,7,5);");//Switch 2 Bypass
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (6,8,5);");//Switch 2 Bypass
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (8,10,5);");//Top Row btw SW1 & SW2
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (10,12,5);");//Top Row btw SW1 & SW2
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (7,9,5);");//Bottom Row btw SW1 & SW2
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (9,11,5);");//Bottom Row btw SW1 & SW2
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (11,13,5);");//Switch 3 Bypass
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (12,13,5);");//Switch 3 Bypass
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (13,14,5);");//SW3 & SW4 Middle
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (14,15,5);");//SW3 & SW4 Middle
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (15,16,5);");//Switch 4 Bypass
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (15,17,5);");//Switch 4 Bypass
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (16,1,5);");//Right Side Middle Connect
      m.exec("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (17,1,5);");//Right Side Middle Connect
      m.exec("SELECT * FROM trackInfoTable;");

      //Create Path_Info Table
      //Table holds track name and ID
       n.exec("CREATE TABLE tracklistingTable (trackID INT, trackNAME TEXT);");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (0,'1-1');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (1,'1-2');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (2,'2-1');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (3,'2-2');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (4,'2-3');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (5,'2-4');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (6,'2-5');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (7,'2-6');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (8,'2-7');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (9,'2-8');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (10,'2-9');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (11,'2-10');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (12,'2-11');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (13,'2-12');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (14,'2-13');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (15,'2-14');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (16,'2-15');");
       n.exec("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (17,'2-16');");
       n.exec("SELECT * FROM tracklistingTable;");

     //This disables createtablesButton and updates text to update user on status.
     ui->createtablesButton->setDisabled(1);
     ui->createtablesButton->setText("Tables in memory!");
     ui->status_update->setText("Tables loading in memory!");
}
