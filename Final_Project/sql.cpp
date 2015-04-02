#include "mainwindow.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>

void MainWindow::sql_query()
{
    QString query = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = 'title'";
    //q = db.exec(query);
    //k = db.exec(query);
    //m = db.exec(query);
    n = db.exec(query);
    q1 = rdb.exec(query);
    //k1 = rdb.exec(query);
    //m1 = rdb.exec(query);
    //n1 = rdb.exec(query);

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

void MainWindow::create_sqltables()
{
    //Connect to SQL Server
    //QSqlTableModel model;
    db = QSqlDatabase::addDatabase( "QSQLITE","Local" );
    db.setHostName("localhost");
    db.setDatabaseName(":memory:");
    if (!db.open())
    {
        qDebug() << "SQL:Error on Local Connection!";
    }

    db.open();

    QString query = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = 'title'";
    //Local Database
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
    INFO = db.exec(query);
    INFO2 = db.exec(query);
    TRAIN = db.exec(query);
    TRAIN2 = db.exec(query);
    LOAD = db.exec(query);
    //Remote Database
    if (rdb.isOpen())
    {
    r1 = rdb.exec(query);
    r2 = rdb.exec(query);
    r3 = rdb.exec(query);
    r4 = rdb.exec(query);
    r5 = rdb.exec(query);
    }


    TRAIN.exec("CREATE TABLE Trains(ID TEXT, START TEXT, Direction TEXT,Destination TEXT,next TEXT,pathID INT);");
    TRAIN.exec("SELECT * FROM Trains;");

    //Create DS_Connectivity Table
    //Table holds track name and ID
    o.exec("CREATE TABLE DS_Connectivity (Current TEXT, NumberOfConnections INT, Connection1 TEXT, Connection2 TEXT, Connection3 TEXT);");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('1.1',2,'2.1','2.2','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('1.2',2,'2.16','2.15','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.1',2,'1.1','2.3','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.2',2,'1.1','2.3','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.3',3,'2.4','2.2','2.1');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.4',2,'2.3','2.5','Null');");
    //o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.5',3,'2.4','2.6','2.7');");
    //TESTING
    //Top Node takes effect -- Bottom Node is ignored
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.5',2,'2.4','2.6','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.5',2,'2.4','2.7','Null');");
    //TESTING
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.6',2,'2.5','2.8','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.7',2,'2.5','2.9','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.8',2,'2.6','2.10','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.9',2,'2.7','2.11','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.10',2,'2.8','2.12','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.11',2,'2.9','2.12','Null');");
    //o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.12',3,'2.13','2.10','2.11');");
    //TESTING
    //Top Node takes effect -- Bottom Node is ignored
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.12',2,'2.13','2.10','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.12',2,'2.13','2.11','Null');");
    //TESTING
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.13',2,'2.12','2.14','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.14',3,'2.13','2.15','2.16');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.15',2,'2.14','1.2','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.16',2,'2.14','1.2','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('4.9',2,'U.3','5.2','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('4.10',2,'U.1','4.11','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('4.11',2,'4.10','5.1','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('4.12',2,'U.2','5.2','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('5.1',2,'4.11','5.3','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('5.2',3,'5.3','4.9','4.12');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('5.3',3,'5.13','5.1','5.2');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('5.4',2,'5.13','U.9','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('5.13',3,'5.4','5.3','5.14');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('5.14',2,'5.13','5.15','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('5.15',2,'5.14','5.16','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('5.16',2,'U.1','5.15','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.1',3,'U.61','4.10','5.16');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.2',1,'4.12','Null','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.3',1,'4.9','Null','Null');");
    //o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.4',2,'U.2','5.2','Null');");
    //o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.5',2,'4.11','5.3','Null');");
    //o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.6',2,'4.10','5.1','Null');");
    //o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.7',2,'U.2','5.2','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.8',2,'U.61','U.10','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.9',2,'U.12','5.4','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.10',2,'U.8','U.13','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.11',1,'U.14','Null','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.12',2,'U.15','U.9','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.13',2,'U.16','U.10','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.14',2,'U.16','U.11','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.15',2,'U.18','U.12','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.16',3,'U.17','U.13','U.14');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.17',2,'U.16','U.19','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.18',2,'U.20','U.15','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.19',2,'U.20','U.17','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.20',3,'U.21','U.18','U.19');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.21',2,'U.22','U.20','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.22',2,'U.31','U.21','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.23',2,'U.32','U.25','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.24',2,'U.26','U.61','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.25',3,'U.23','U.21','1.4R');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.26',3,'U.24','U.27','U.35');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.27',3,'U.26','U.36','U.37');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.28',1,'U.42','Null','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.29',1,'U.41','Null','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.30',1,'U.40','Null','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.31',2,'U.33','U.22','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.32',2,'U.34','U.23','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.33',2,'U.44','U.31','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.34',3,'U.32','1.3R','U.44');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.35',3,'U.62','U.26','U.36');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.36',3,'U.35','U.27','U.37');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.37',3,'U.36','U.27','U.38');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.38',3,'U.37','U.42','U.39');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.39',3,'U.38','U.40','U.41');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.40',2,'U.39','U.30','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.41',2,'U.39','U.29','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.42',2,'U.38','U.28','Null');");
    //o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.43',2,'U.2','5.2','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.44',3,'U.45','U.33','U.34');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.45',3,'U.44','U.46','U.47');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.46',2,'U.50','U.45','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.47',2,'U.48','U.45','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.48',2,'U.53','U.47','Null');");
    //o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.49',2,'U.3','5.2','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.50',2,'U.51','U.46','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.51',3,'U.50','U.52','U.54');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.52',2,'U.51','U.55','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.53',2,'U.48','U.57','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.54',2,'U.51','U.56','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.55',2,'U.52','U.58','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.56',1,'U.54','Null','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.57',3,'U.53','U.59','U.63');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.58',2,'U.55','U.59','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.59',3,'U.60','U.57','U.58');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.60',3,'U.59','1.3R','U.62');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.61',3,'U.8','U.24','1.4R');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.62',2,'U.60','U.35','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('U.63',2,'U.57','U.1','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('1.3R',2,'U.60','U.34','Null');");
    o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('1.4R',2,'U.25','U.61','Null');");

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
     l.exec("CREATE TABLE pathInfoTable (pathID INT, nextpathID INT, nextID1 TEXT, nextID2 TEXT, nextID3 TEXT, nextID4 TEXT, nextID5 TEXT, nextID6 TEXT, nextID7 TEXT, nextID8 TEXT, nextID9 TEXT, nextID10 TEXT);");
     l.exec("SELECT * FROM pathInfoTable;");
}

