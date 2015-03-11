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

    ui->viewtableButton->setDisabled(1);
    ui->viewtableButton->setText("Waiting for SQL Data");

    this->setWindowTitle("Testing_Framework");

    //Connect statements for Algorithm Testing
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(dij_mainprogram()));
    connect(ui->viewtableButton,SIGNAL(clicked()),this,SLOT(VIEWTABLE()));
    connect(ui->swapButton,SIGNAL(clicked()),this,SLOT(SWAP()));

    //Connect statements for SQL Testing
    connect(ui->queryButton,SIGNAL(clicked()),this,SLOT(sql_query()));
    connect(ui->createtablesButton,SIGNAL(clicked()),this,SLOT(create_sqltables()));
    connect(ui->server_connectButton,SIGNAL(clicked()),this,SLOT(sqlserver_connect()));
}

void Dialog::sql_query()
{
    //Connect to SQL Server
    //QSqlTableModel model;
    db = QSqlDatabase::addDatabase( "QSQLITE" ,"Local" );
    db.setDatabaseName("Local");
    db.setHostName("localhost");
    db.setDatabaseName(":memory:");
    if (!db.open())
    {
        qDebug() << "Error";
    }

    db.open();

    rdb = QSqlDatabase::addDatabase( "QMYSQL", "Remote" );
    rdb.setDatabaseName("Remote");
    rdb.setHostName("pavelow.eng.uah.edu");
    rdb.setPort(33158);
    rdb.setDatabaseName("team4b");
    rdb.setUserName("root");
    rdb.setPassword("drabroig");
    if (!rdb.open())
    {
        qDebug() << "Error";
    }

    rdb.open();

    QString query = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = 'title'";
    q = db.exec(query);
    k = db.exec(query);
    m = db.exec(query);
    n = db.exec(query);
    q1 = rdb.exec(query);
    k1 = rdb.exec(query);
    m1 = rdb.exec(query);
    n1 = rdb.exec(query);

    //Create Path_Info Table
    //Table holds track name and ID
     n.exec("CREATE TABLE tracklistingTable (trackID INT, trackNAME TEXT);");

    //Populate switches
    /*-------------------------------------------------------------------*/
        QString s = QString("SELECT Current from %3").arg("DS_Connectivity");
        q1 = db.exec(s);
        ii = 0;
    for(;q1.next() == 1;) //If it is 1 it contains data
    {
        QString ss1 = q1.value(0).toString();
        int ss2 = ii;
        //qDebug() << "(" << ss1 << ", " << ss2 << ")";
        QString ss = QString("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (%1,'%2');").arg(ss2).arg(ss1);
        n.exec(ss);
        ii++;
    }
    /*-------------------------------------------------------------------*/

   n.exec("SELECT * FROM tracklistingTable;");

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

void Dialog::VIEWTABLE()
{
    const QString& VIEW = ui->listWidget->currentItem()->text();
    if(VIEW=="trainInfoTable")
    {
        model = new QSqlQueryModel;
        model->setQuery(k);
        view = new QTableView;
        view->setModel(model);
        view->show();
    }
    else if(VIEW=="trackInfoTable")
    {
        model = new QSqlQueryModel;
        model->setQuery(qq);
        view = new QTableView;
        view->setModel(model);
        view->show();
    }
    else if(VIEW=="tracklistingTable")
    {
        model = new QSqlQueryModel;
        model->setQuery(n);
        view = new QTableView;
        view->setModel(model);
        view->show();
    }
    else if(VIEW=="pathInfoTable")
    {
        model = new QSqlQueryModel;
        model->setQuery(l);
        view = new QTableView;
        view->setModel(model);
        view->show();
    }
    else if(VIEW=="switchInfoTable")
    {
        model = new QSqlQueryModel;
        model->setQuery(q);
        view = new QTableView;
        view->setModel(model);
        view->show();
    }
    else if(VIEW=="throttleInfoTable")
    {
        model = new QSqlQueryModel;
        model->setQuery(j);
        view = new QTableView;
        view->setModel(model);
        view->show();
    }
    else if(VIEW=="DS_Connectivity")
    {
        model = new QSqlQueryModel;
        model->setQuery(o);
        view = new QTableView;
        view->setModel(model);
        view->show();
    }
    else{}
}

void Dialog::sqlserver_connect()
{
    ui->viewtableButton->setDisabled(0);
    ui->viewtableButton->setText("View Tables");
    sql_query();
}

void Dialog::SWAP()
{
    //Starting position
    startpick = ui->sourceBox->currentText();

    //Destination
    destpick = ui->destBox->currentText();

    QString Swap = destpick;
    ui->destBox->setCurrentText(startpick);
    ui->sourceBox->setCurrentText(Swap);
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
    o = db.exec(query);
    p = db.exec(query);
    mm = db.exec(query);
    nn = db.exec(query);
    qq = db.exec(query);

    //Create DS_Connectivity Table
    //Table holds track name and ID
    o.exec("CREATE TABLE DS_Connectivity (Current TEXT, NumberOfConnections INT, Connection1 TEXT, Connection2 TEXT, Connection3 TEXT);");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('1.1',2,'2.1','2.3','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('1.2',2,'2.16','2.15','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.1',2,'2.1','2.3','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.2',2,'2.1','2.3','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.3',3,'2.4','2.2','2.1');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.4',2,'2.3','2.5','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.5',3,'2.4','2.7','2.6');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.6',2,'2.5','2.8','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.7',2,'2.5','2.9','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.8',2,'2.6','2.10','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.9',2,'2.7','2.11','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.10',2,'2.8','2.12','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.11',2,'2.9','2.12','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.12',3,'2.13','2.11','2.10');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.13',2,'2.12','2.14','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.14',3,'2.13','2.15','2.16');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.15',2,'2.14','1.2','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.16',2,'2.14','1.2','Null');");
    o.exec("SELECT * FROM DS_Connectivity;");

    n.exec("CREATE TABLE tracklistingTable (trackID INT, trackNAME TEXT);");
    int ii = 0;
    /*-------------------------------------------------------------------*/
        QString s3 = QString("SELECT Current from %3").arg("DS_Connectivity");
        p = db.exec(s3);

    //For each swich in switchInfoTable execute the following:
    for(;p.next() == 1;) //If it is 1 it contains data
    {
        int ss1 = ii;
        QString ss2 = p.value(0).toString();
        QString ss = QString("INSERT INTO tracklistingTable (trackID, trackNAME) VALUES (%1,'%2');").arg(ss1).arg(ss2);
        n.exec(ss);
        ii++;
    }
    /*-------------------------------------------------------------------*/
    n.exec("SELECT * FROM tracklistingTable;");

    qq.exec("CREATE TABLE trackInfoTable (currentnode INT, nextnode INT, weight INT);");
    /*-------------------------------------------------------------------*/
        QString s9 = QString("SELECT Current,NumberOfConnections,Connection1,Connection2,Connection3 from %3").arg("DS_Connectivity");
        o = db.exec(s9);
        QString smi3 = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");

    for(;o.next() == 1;) //Check DS_Connectivity table for data
    {
        n = db.exec(smi3);
        for(;n.next() == 1;) //Check each track piece
        {
            int trackID = n.value(0).toInt(); //trackID
            QString trackNAME = n.value(1).toString();//trackNAME

                QString Current = o.value(0).toString();//Current
                NumofConn = o.value(1).toInt();   //Connection1
                QString Conn1 = o.value(2).toString();//Connection2
                QString Conn2 = o.value(3).toString();//Connection3
                QString Conn3 = o.value(4).toString();//Connection3
                //qDebug() << Current <<","<< NumofConn <<","<< Conn1 <<","<< Conn2 <<","<< Conn3;

            //Converts strings to int values
            if(Current == trackNAME)
            {
                COL0 = trackID;
            }
            if(Conn1 == trackNAME)
            {
                COL1 = trackID;
            }
            if(Conn2 == trackNAME)
            {
                COL2 = trackID;
            }
            if(Conn3 == trackNAME)
            {
                COL3 = trackID;
            }

        }
        //qDebug() << COL0 <<","<< COL1 <<","<< COL2 <<","<< COL3;

        if (COL0 < COL1 && COL0 < COL2)
        {
                QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
                qq.exec(smpms);
                QString smpms1 = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL1).arg(COL0);
                qq.exec(smpms1);
                QString smpms2 = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL2);
                qq.exec(smpms2);
                QString smpms3 = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL2).arg(COL0);
                qq.exec(smpms3);
        }
        else
        {
        if (COL0 < COL1)
        {
        if (COL0 != COL1)
        {
            QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
            qq.exec(smpms);

        }
        }
        if (COL0 < COL2)
        {
        if (COL0 != COL2)
        {
            QString smpms1 = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL2);
            qq.exec(smpms1);
        }
        }
        if (NumofConn == 3)
        {
        if (COL0 < COL3)
        {
        if (COL0 != COL3)
        {
            QString smpms1 = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL3);
            qq.exec(smpms1);
        }
        }
        }
        }

    }
    /*-------------------------------------------------------------------*/
    qq.exec("SELECT * FROM trackInfoTable;");

    QString ts3 = QString("SELECT trackID,trackNAME from %1").arg("tracklistingTable");
    n = db.exec(ts3);

    /*-------------------------------------------------------------------*/
    for(;n.next() == 1;) //If it is 1 it contains data
    {
    //int sts1 = n.value(0).toInt();
    QString sts2 = n.value(1).toString();

    ui->sourceBox->addItem(sts2);
    ui->destBox->addItem(sts2);

    }
    /*-------------------------------------------------------------------*/

    //Create Switch_Info Table
    //Table list switches and Open/Closed status
    //Unused but code was created to read from this table and do nothing with it
     p.exec("CREATE TABLE switchInfoTable (switch TEXT, position TEXT);");
     p.exec("INSERT INTO switchInfoTable VALUES ('SW1', 'Open');");
     p.exec("INSERT INTO switchInfoTable VALUES ('SW2', 'Open');");
     p.exec("INSERT INTO switchInfoTable VALUES ('SW3', 'Open');");
     p.exec("INSERT INTO switchInfoTable VALUES ('SW4', 'Open');");
     p.exec("SELECT * FROM switchInfoTable;");

    //Create Throttle_Info Table
    //CURRENTLY NOT IMPORTANT
     j.exec("CREATE TABLE throttleInfoTable (train TEXT, direction TEXT, throttle INT);");
     j.exec("INSERT INTO throttleInfoTable (train, direction, throttle) VALUES ('Alabama Express', 'Backward', 1);");
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

     //This disables createtablesButton and updates text to update user on status.
     ui->createtablesButton->setDisabled(1);
     ui->createtablesButton->setText("Tables in memory!");
     ui->status_update->setText("Tables loading in memory!");
     ui->viewtableButton->setDisabled(0);
     ui->viewtableButton->setText("View Tables");
}
