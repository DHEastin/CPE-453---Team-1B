#include "train_dialog.h"
#include "ui_train_dialog.h"
#include "mainwindow.h"

Train_Dialog::Train_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Train_Dialog)
{
    ui->setupUi(this);

    //ui->startBox->currentText();

    this->setWindowTitle("Add Train");

    connect(ui->startBox,SIGNAL(currentTextChanged(QString)),this,SLOT(Forward_Function()));
    connect(ui->startBox,SIGNAL(currentTextChanged(QString)),this,SLOT(Backward_Function()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(SaveTrainID()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
    //connect(ui->startBox,SIGNAL(currentIndexChanged(QString))

    LOCAL();
    //Change to REMOTE(); when you want to test real live data.

}

Train_Dialog::~Train_Dialog()
{
    delete ui;
}

void Train_Dialog::LOCAL()
{
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
    ui->startBox->addItem(sts1);
    }
}

void Train_Dialog::REMOTE()
{
    rdb = QSqlDatabase::addDatabase( "QMYSQL");
    rdb.setDatabaseName("Remote");
    rdb.setHostName("pavelow.eng.uah.edu");
    rdb.setPort(33158);
    rdb.setDatabaseName("team4b");
    rdb.setUserName("root");
    rdb.setPassword("drabroig");
    if (!rdb.open())
    {
        qDebug() << "Train_Dia:Error on Remote Connection!";
    }
    if (rdb.open())
    {
        qDebug() << "Train_Dia:Remote Connection Connected!";
    }

    rdb.open();

    QString ts3 = QString("SELECT Current from %1").arg("DS_Connectivity");
    n = rdb.exec(ts3);

    /*-------------------------------------------------------------------*/
    for(;n.next() == 1;) //If it is 1 it contains data
    {
    QString sts1 = n.value(0).toString();
    ui->startBox->addItem(sts1);
    }
    rdb.close();
}

void Train_Dialog::Forward_Function()
{
    QString ts3 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current=%2").arg("DS_Connectivity").arg(ui->startBox->currentText());
    o = db.exec(ts3);
    o.next();
    QString sts1 = o.value(2).toString();
    ui->forwardButton->setText(sts1);
}

void Train_Dialog::Backward_Function()
{
    QString ts3 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current=%2").arg("DS_Connectivity").arg(ui->startBox->currentText());
    p = db.exec(ts3);
    p.next();
    QString sts1 = p.value(3).toString();
    ui->backwardButton->setText(sts1);
}

void Train_Dialog::SaveTrainID()
{
    /*
    //QString DIRECTION;
    ID::TrainID = ui->train_idEdit->text();
    //QString TrainID = ui->train_idEdit->text();
    if(ui->forwardButton->isChecked())
    {
        ID::DIRECTION = ui->forwardButton->text();
    }
    else if(ui->backwardButton->isChecked())
    {
        ID::DIRECTION = ui->backwardButton->text();
    }
    else
    {
        qDebug() << "No Button is checked.";
        ID::DIRECTION = "NULL";
    }
    qDebug() << ID::TrainID<<","<<ID::DIRECTION;

    //return TrainID;
    */
}

