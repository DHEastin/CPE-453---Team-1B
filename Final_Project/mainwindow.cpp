#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Train Scheduling Application");

    create_sqltables();

    QString ts1 = QString("SELECT Current from %1").arg("DS_Connectivity");
    n = db.exec(ts1);

    /*-------------------------------------------------------------------*/
    for(;n.next() == 1;) //If it is 1 it contains data
    {
    QString sts1 = n.value(0).toString();
    ui->destinationBox->addItem(sts1);
    }

    connect(ui->actionAdd_Train,SIGNAL(triggered()),this,SLOT(Add_Train()));
    connect(ui->actionEdit_Train,SIGNAL(triggered()),this,SLOT(Edit_Train()));
    connect(ui->actionDelete_Train,SIGNAL(triggered()),this,SLOT(Delete_Train()));
    connect(ui->actionSave_State,SIGNAL(triggered()),this,SLOT(Save_State()));
    connect(ui->actionLoad_State,SIGNAL(triggered()),this,SLOT(Load_State()));
    connect(ui->scheduleButton,SIGNAL(clicked()),this,SLOT(Schedule()));
    connect(ui->actionView_Trains,SIGNAL(triggered()),this,SLOT(Train_Table()));
    connect(ui->set_scheduleButton,SIGNAL(clicked()),this,SLOT(Set_Schedule()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Add_Train()
{
    items.clear();
    ID = QInputDialog::getText(this, tr("Create Train ID"),
                                         tr("TrainID:"), QLineEdit::Normal,
                                         "Train 1", &ok);

    QString ts0 = QString("SELECT Current from %1").arg("DS_Connectivity");
    LOAD = db.exec(ts0);

    /*-------------------------------------------------------------------*/
    for(;LOAD.next() == 1;) //If it is 1 it contains data
    {
    QString sts0 = LOAD.value(0).toString();
    items.append(sts0);
    }

    Start = QInputDialog::getItem(this, tr("Select Starting Point"),
                                         tr("Starting Point:"), items, 0, false, &ok);

    items.clear();
    //Forward
    QString ts1 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current=%2").arg("DS_Connectivity").arg(Start);
    LOAD = db.exec(ts1);
    LOAD.next();
    QString sts1 = LOAD.value(2).toString();
    items.append(sts1);

    //Backward
    QString ts2 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current=%2").arg("DS_Connectivity").arg(Start);
    LOAD = db.exec(ts2);
    LOAD.next();
    QString sts3 = LOAD.value(3).toString();
    items.append(sts3);

    direction = QInputDialog::getItem(this, tr("Choose Direction"),
                                         tr("Direction:"), items, 0, false, &ok);
    if (ok && !ID.isEmpty())
        ui->trainBox->addItem(ID);

    QString qtts0 = QString("INSERT INTO Trains (ID,Start,Direction,Destination) VALUES ('%1','%2','%3','%4')").arg(ID).arg(Start).arg(direction).arg("");
    TRAIN = db.exec(qtts0);
    TRAIN.next();
}

void MainWindow::Edit_Train()
{
    ID = ui->trainBox->currentText();
    QString VAL1,VAL2,VAL3;
    items.clear();
    QString ts4 = QString("SELECT ID,Start,Direction from %1 WHERE ID=%2").arg("Trains").arg(ID);
    INFO = db.exec(ts4);

    /*-------------------------------------------------------------------*/
    for(;INFO.next() == 1;) //If it is 1 it contains data
    {
    VAL1 = INFO.value(0).toString();
    VAL2 = INFO.value(1).toString();
    VAL3 = INFO.value(2).toString();
    }

    QString ts0 = QString("SELECT Current from %1").arg("DS_Connectivity");
    o = db.exec(ts0);

    /*-------------------------------------------------------------------*/
    for(;o.next() == 1;) //If it is 1 it contains data
    {
    QString sts0 = o.value(0).toString();
    items.append(sts0);
    }

    Start = QInputDialog::getItem(this, tr("Select Starting Point"),
                                         tr("Starting Point:"), items, 0, false, &ok);

    items.clear();
    //Forward
    QString Sts1 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current=%2").arg("DS_Connectivity").arg(Start);
    o = db.exec(Sts1);
    o.next();
    QString sts1 = o.value(2).toString();
    items.append(sts1);

    //Backward
    QString Sts2 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current=%2").arg("DS_Connectivity").arg(Start);
    o = db.exec(Sts2);
    o.next();
    QString sts3 = o.value(3).toString();
    items.append(sts3);

    direction = QInputDialog::getItem(this, tr("Choose Direction"),
                                         tr("Direction:"), items, 0, false, &ok);

    QString tts0 = QString("UPDATE Trains SET Start='%1',Direction='%2' WHERE ID='%3'").arg(Start).arg(direction).arg(ID);
    qDebug() << tts0;
    TRAIN = db.exec(tts0);

}

void MainWindow::Delete_Train()
{
   int CI_Train = ui->trainBox->currentIndex();
   QString CT_Train = ui->trainBox->currentText();
   ui->trainBox->removeItem(CI_Train);

   QString tts0 = QString("DELETE FROM Trains WHERE ID='%1'").arg(CT_Train);
   INFO = db.exec(tts0);
}

void MainWindow::Load_State()
{

}

void MainWindow::Save_State()
{

}

void MainWindow::Set_Schedule()
{
     Destination = ui->destinationBox->currentText();
     QString ptts0 = QString("UPDATE Trains SET Destination='%1' WHERE ID='%2'").arg(Destination).arg(ID);
     TRAIN = db.exec(ptts0);
}


void MainWindow::Schedule()
{

}

void MainWindow::Train_Table()
{
    QSqlTableModel* model = new QSqlTableModel( this, db );
    QTableView *view = new QTableView;
    view->setModel(model);
    model->setTable( "Trains" );
    model->select();
    view->show();
}



