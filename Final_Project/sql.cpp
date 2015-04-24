#include "mainwindow.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>

void MainWindow::sql_query()
{
    QString query = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = 'title'";
    n = db.exec(query);
    q1 = rdb.exec(query);

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
    Train1 = db.exec(query);
    Train2 = db.exec(query);
    Train3 = db.exec(query);
    TrainSW1 = db.exec(query);
    TrainSW2 = db.exec(query);
    TrainSW3 = db.exec(query);
    TrainSW4 = db.exec(query);
    TrainSW5 = db.exec(query);
    runSchedQuery = db.exec(query);

    l1 = db.exec(query);
    l2 = db.exec(query);
    l3 = db.exec(query);
    l4 = db.exec(query);
    l5 = db.exec(query);

    //Remote Database
    if (rdb.isOpen())
    {
    r1 = rdb.exec(query);
    r2 = rdb.exec(query);
    r3 = rdb.exec(query);
    r4 = rdb.exec(query);
    r5 = rdb.exec(query);
    overrideCheck = rdb.exec(query);
    }


    // 0 = Open --- 1 = Bypass
    //Create Switch_Info Table
    //Table list switches and Open/Closed status
     p.exec("CREATE TABLE switchInfoTable (switch TEXT, position INT, straight TEXT, openPOS TEXT, closedPOS TEXT);");
     p.exec("SELECT * FROM switchInfoTable;");

    TRAIN.exec("CREATE TABLE Trains(ID TEXT, START TEXT, Direction TEXT,Destination TEXT,next TEXT,pathID INT);");
    TRAIN.exec("SELECT * FROM Trains;");

    TrainSW1.exec("CREATE TABLE Trains1(switch TEXT, position INT);");
    TrainSW1.exec("SELECT * FROM Trains1;");

    TrainSW2.exec("CREATE TABLE Trains2(switch TEXT, position INT);");
    TrainSW2.exec("SELECT * FROM Trains2;");

    TrainSW3.exec("CREATE TABLE Trains3(switch TEXT, position INT);");
    TrainSW3.exec("SELECT * FROM Trains3;");

    TrainSW4.exec("CREATE TABLE Trains4(switch TEXT, position INT);");
    TrainSW4.exec("SELECT * FROM Trains4;");

    TrainSW5.exec("CREATE TABLE Trains5(switch TEXT, position INT);");
    TrainSW5.exec("SELECT * FROM Trains5;");

     //SwitchDIR == Direction of Straight part of Switch

     o.exec("CREATE TABLE DS_Connectivity (Current TEXT, NumberOfConnections INT, Connection1 TEXT, Connection2 TEXT, Connection3 TEXT, SwitchDIR TEXT);");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-1',2,'2-2','2-1','Null','B');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-2',2,'2-16','2-15','Null','B');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-3',2,'3-16','4-4','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-4',2,'8-16','8-4','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-5',3,'1-6','4-2','3-3','W');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-6',3,'3-16','3-4','1-5','W');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-7',3,'1-8','4-3','4-4','W');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-8',3,'1-7','1-14','1-15','E');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-9',1,'1-12','Null','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-10',2,'4-16','3-15','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-11',2,'3-13','4-13','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-12',2,'4-15','1-9','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-14',2,'4-14','1-8','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('1-15',2,'4-13','1-8','Null','Null');");

     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-1',2,'2-3','1-1','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-2',2,'2-3','1-1','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-3',3,'2-4','2-2','2-1','IE');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-4',2,'2-5','2-3','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-5',3,'2-4','2-7','2-6','IW');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-6',2,'2-8','2-5','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-7',2,'2-9','2-5','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-8',2,'2-10','2-6','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-9',2,'2-11','2-7','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-10',2,'2-12','2-8','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-11',2,'2-12','2-9','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-12',3,'2-13','2-11','2-10','IE');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-13',2,'2-14','2-12','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-14',3,'2-13','2-15','2-16','IW');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-15',2,'1-2','2-14','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('2-16',2,'1-2','2-14','Null','Null');");

     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('3-1',2,'6-4','4-2','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('3-3',2,'6-3','1-5','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('3-4',2,'6-2','1-6','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('3-5',1,'4-9','Null','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('3-6',3,'3-8','5-16','4-10','BW');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('3-7',1,'4-12','Null','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('3-8',2,'3-6','3-13','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('3-13',3,'1-11','3-8','3-15','SL');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('3-14',2,'1-10','3-15','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('3-15',3,'3-16','3-14','3-13','NL');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('3-16',3,'3-15','1-3','1-6','SL');");

     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('4-2',3,'1-5','5-8','3-1','W');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('4-3',2,'1-7','6-5','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('4-4',3,'6-6','1-3','1-7','E');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('4-9',2,'5-2','3-5','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('4-10',2,'4-11','3-6','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('4-11',2,'5-1','4-10','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('4-12',2,'5-2','3-7','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('4-13',2,'1-11','1-15','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('4-14',2,'4-15','1-14','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('4-15',3,'4-14','4-16','1-12','NL');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('4-16',2,'4-15','1-10','Null','Null');");

     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('5-1',2,'5-3','4-11','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('5-2',3,'5-3','4-9','4-12','BE);");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('5-3',3,'5-13','5-2','5-1','BE');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('5-4',2,'8-1','5-13','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('5-5',2,'5-8','7-4','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('5-6',2,'5-7','7-3','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('5-7',3,'5-8','7-1','5-6','W');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('5-8',3,'4-2','5-7','5-5','W');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('5-13',3,'5-4','5-14','5-3','BE');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('5-14',2,'5-13','5-15','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('5-15',2,'5-14','5-16','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('5-16',2,'5-15','3-6','Null','Null');");

     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('6-1',3,'8-15','6-4','6-3','E');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('6-2',2,'8-15','3-4','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('6-3',2,'6-1','3-3','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('6-4',2,'6-1','3-1','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('6-5',2,'4-3','8-14','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('6-6',2,'4-4','8-13','Null','Null');");

     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('7-1',2,'5-7','7-2','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('7-2',1,'7-1','Null','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('7-3',1,'5-6','Null','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('7-4',1,'5-5','Null','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('7-5',3,'7-7','7-8','8-5','NR');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('7-6',2,'7-13','8-6','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('7-7',2,'8-11','7-5','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('7-8',2,'8-2','7-5','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('7-13',2,'8-10','7-6','Null','Null');");

     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-1',3,'8-6','5-4','8-3','SR');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-2',2,'8-3','7-8','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-3',3,'8-4','8-2','8-1','NR');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-4',3,'8-3','1-4','8-9','SR');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-5',2,'7-5','8-8','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-6',2,'7-6','8-1','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-8',1,'8-5','Null','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-9',3,'8-10','8-14','8-16','E');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-10',3,'8-9','8-11','7-13','W');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-11',2,'8-10','7-7','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-13',2,'6-6','8-16','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-14',2,'6-5','8-9','Null','Null');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-15',3,'8-4','6-2','6-1','E');");
     o.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3, SwitchDIR) VALUES ('8-16',3,'8-13','1-4','8-9','W');");


     o.exec("SELECT * FROM DS_Connectivity;");

    n.exec("CREATE TABLE tracklistingTable (trackID INT, trackNAME TEXT);");
    int ii = 0;
    /*-------------------------------------------------------------------*/
        QString s3 = QString("SELECT Current from %3").arg("DS_Connectivity");
        p = db.exec(s3);

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
        QString s9 = QString("SELECT Current,NumberOfConnections,Connection1,Connection2,Connection3,SwitchDIR from %3").arg("DS_Connectivity");
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
                SwitchDIR = o.value(5).toString();//Switch Direction

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

        //Tested and Works
        if(NumofConn == 1) //Dead-Ends
        {
            QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
            qq.exec(smpms);
            QString smpms1 = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL1).arg(COL0);
            qq.exec(smpms1);
        }

        if(NumofConn == 2)
        {
            QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
            qq.exec(smpms);
        }

        if(NumofConn == 3) //Switches
        {
            QString smpms = QString("INSERT INTO switchInfoTable (switch,position,straight,openPOS,closedPOS) VALUES ('%1',0,'%2','%3','%4');").arg(COL0).arg(COL1).arg(COL2).arg(COL3);
            p.exec(smpms);
            if(SwitchDIR == "NL")
            {
                QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
                qq.exec(smpms);
            }
            if(SwitchDIR == "SL")
            {
                //QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
                //qq.exec(smpms);
            }
            if(SwitchDIR == "NR")
            {
                //QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
                //qq.exec(smpms);
            }
            if(SwitchDIR == "SR")
            {
                QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
                qq.exec(smpms);
            }
            if(SwitchDIR == "E")
            {
                //QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
                //qq.exec(smpms);
            }
            if(SwitchDIR == "W")
            {
                QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
                qq.exec(smpms);
            }
            if(SwitchDIR == "IE")
            {
                QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
                qq.exec(smpms);
            }
            if(SwitchDIR == "IW")
            {
                //QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
                //qq.exec(smpms);
            }
            if(SwitchDIR == "BE")
            {
                QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
                qq.exec(smpms);
            }
            if(SwitchDIR == "BW")
            {
                //QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
                //qq.exec(smpms);
            }
        }

        /*
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
        //if (COL0 < COL3)
        //{
        if (COL0 != COL3)
        {
            //QString smpms1 = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL0).arg(COL1);
            //qq.exec(smpms1);
            QString smpms2 = QString("INSERT INTO switchInfoTable (switch,position,straight,openPOS,closedPOS) VALUES ('%1',0,'%2','%3','%4');").arg(COL0).arg(COL1).arg(COL2).arg(COL3);
            p.exec(smpms2);
        }
        //}
        }
        }
    */
    }

    //Create Throttle_Info Table
    //CURRENTLY NOT IMPORTANT
     j.exec("CREATE TABLE throttleInfoTable (train TEXT, direction TEXT, throttle INT);");
     j.exec("SELECT * FROM throttleInfoTable;");

    //Create Train_Info Table
    //CURRENTLY NOT IMPORTANT
     k.exec("CREATE TABLE trainInfoTable (train TEXT, currentOcc TEXT, destination TEXT, next TEXT, path INT);");
     k.exec("SELECT * FROM trainInfoTable;");

    //Create Path_Info Table
     l.exec("CREATE TABLE pathInfoTable (pathID INT, nextpathID INT, nextID1 TEXT, nextID2 TEXT, nextID3 TEXT, nextID4 TEXT, nextID5 TEXT, nextID6 TEXT, nextID7 TEXT, nextID8 TEXT, nextID9 TEXT, nextID10 TEXT);");
     l.exec("SELECT * FROM pathInfoTable;");
}

