#include "schedule_dialog.h"
#include "ui_schedule_dialog.h"
#include "mainwindow.h"
#include <QComboBox>

Schedule_Dialog::Schedule_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Schedule_Dialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Train Scheduling");

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(Save_Schedule()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(reject()));

    LOCAL();
    //Change to REMOTE(); when you want to test real live data.

}

Schedule_Dialog::~Schedule_Dialog()
{
    delete ui;
}

void Schedule_Dialog::LOCAL()
{
    db.close();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName(":memory:");
    if (!db.open())
    {
        qDebug() << "SQL:Error on Local Connection!";
    }

    db.open();

    QString query = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = 'title'";
    n = db.exec(query);

    //Create DS_Connectivity Table
    //Table holds track name and ID
    n.exec("CREATE TABLE DS_Connectivity (Current TEXT, NumberOfConnections INT, Connection1 TEXT, Connection2 TEXT, Connection3 TEXT);");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('1.1',2,'2.1','2.2','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('1.2',2,'2.16','2.15','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.1',2,'1.1','2.3','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.2',2,'1.1','2.3','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.3',3,'2.4','2.2','2.1');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.4',2,'2.3','2.5','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.5',3,'2.4','2.7','2.6');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.6',2,'2.5','2.8','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.7',2,'2.5','2.9','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.8',2,'2.6','2.10','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.9',2,'2.7','2.11','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.10',2,'2.8','2.12','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.11',2,'2.9','2.12','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.12',3,'2.13','2.11','2.10');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.13',2,'2.12','2.14','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.14',3,'2.13','2.15','2.16');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.15',2,'2.14','1.2','Null');");
    n.exec("INSERT INTO DS_Connectivity (Current, NumberOfConnections, Connection1, Connection2, Connection3) VALUES ('2.16',2,'2.14','1.2','Null');");
    n.exec("SELECT * FROM DS_Connectivity;");

    QString ts3 = QString("SELECT Current from %1").arg("DS_Connectivity");
    n = db.exec(ts3);

    /*-------------------------------------------------------------------*/
    for(;n.next() == 1;) //If it is 1 it contains data
    {
    QString sts1 = n.value(0).toString();
    ui->destBox->addItem(sts1);
    }
    db.close();
}

void Schedule_Dialog::REMOTE()
{
    rdb = QSqlDatabase::addDatabase("QMYSQL");
    rdb.setDatabaseName("Remote");
    rdb.setHostName("pavelow.eng.uah.edu");
    rdb.setPort(33158);
    rdb.setDatabaseName("team4b");
    rdb.setUserName("root");
    rdb.setPassword("drabroig");
    if (!rdb.open())
    {
        qDebug() << "Sch_Dia:Error on Remote Connection!";
    }
    if (rdb.open())
    {
        qDebug() << "Sch_Dia:Remote Connection Connected!";
    }

    rdb.open();

    QString ts3 = QString("SELECT Current from %1").arg("DS_Connectivity");
    n = rdb.exec(ts3);

    for(;n.next() == 1;) //If it is 1 it contains data
    {
    QString sts1 = n.value(0).toString();
    ui->destBox->addItem(sts1);
    }
    rdb.close();
}

void Schedule_Dialog::Save_Schedule()
{
    //Train_Dialog::SaveTrainID();// = new Train_Dialog;
    //Train_Dialog::SaveTrainID();
    //QString Train = Train_Dialog.TrainID;
    DESTINATION= ui->destBox->currentText();
    //ui->trainSelection->addItem(Train);

    qDebug() << DESTINATION;
}
