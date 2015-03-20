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
                                         QDir::home().dirName(), &ok);

    QString ts0 = QString("SELECT Current from %1").arg("DS_Connectivity");
    n = db.exec(ts0);

    /*-------------------------------------------------------------------*/
    for(;n.next() == 1;) //If it is 1 it contains data
    {
    QString sts0 = n.value(0).toString();
    items.append(sts0);
    }

    Start = QInputDialog::getItem(this, tr("Select Starting Point"),
                                         tr("Starting Point:"), items, 0, false, &ok);

    items.clear();
    //Forward
    QString ts1 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current=%2").arg("DS_Connectivity").arg(Start);
    p = db.exec(ts1);
    p.next();
    QString sts1 = p.value(2).toString();
    items.append(sts1);

    //Backward
    QString ts2 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current=%2").arg("DS_Connectivity").arg(Start);
    p = db.exec(ts2);
    p.next();
    QString sts3 = p.value(3).toString();
    items.append(sts3);

    direction = QInputDialog::getItem(this, tr("Choose Direction"),
                                         tr("Direction:"), items, 0, false, &ok);
    if (ok && !ID.isEmpty())
        ui->trainBox->addItem(ID);

    if (ok && !Start.isEmpty())
        qDebug() << Start;

    if (ok && !direction.isEmpty())
        qDebug() << direction;

    QString tts0 = QString("INSERT INTO Trains (ID,Start,Direction) VALUES (%1,%2,%3)").arg(ID).arg(Start).arg(direction);
    INFO = db.exec(tts0);
}

void MainWindow::Edit_Train()
{
    QString VAL1,VAL2,VAL3;
    items.clear();
    QString ts4 = QString("SELECT ID,Start,Direction from %1 WHERE ID=%2").arg("Trains").arg(ID);
    n = db.exec(ts4);

    /*-------------------------------------------------------------------*/
    for(;n.next() == 1;) //If it is 1 it contains data
    {
    VAL1 = n.value(0).toString();
    VAL2 = n.value(1).toString();
    VAL3 = n.value(2).toString();
    }

    QString ts0 = QString("SELECT Current from %1").arg("DS_Connectivity");
    n = db.exec(ts0);

    /*-------------------------------------------------------------------*/
    for(;n.next() == 1;) //If it is 1 it contains data
    {
    QString sts0 = n.value(0).toString();
    items.append(sts0);
    }

    Start = QInputDialog::getItem(this, tr("Select Starting Point"),
                                         tr("Starting Point:"), items, 0, false, &ok);

    items.clear();
    //Forward
    QString Sts1 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current=%2").arg("DS_Connectivity").arg(Start);
    p = db.exec(Sts1);
    p.next();
    QString sts1 = p.value(2).toString();
    items.append(sts1);

    //Backward
    QString Sts2 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current=%2").arg("DS_Connectivity").arg(Start);
    p = db.exec(Sts2);
    p.next();
    QString sts3 = p.value(3).toString();
    items.append(sts3);

    direction = QInputDialog::getItem(this, tr("Choose Direction"),
                                         tr("Direction:"), items, 0, false, &ok);

    if (ok && !Start.isEmpty())
        qDebug() << Start;

    if (ok && !direction.isEmpty())
        qDebug() << direction;

    QString tts0 = QString("UPDATE Trains SET Start=%1,SET Direction=%2 WHERE ID=%3").arg(Start).arg(direction).arg(ID);
    INFO = db.exec(tts0);

}

void MainWindow::Delete_Train()
{
   int CI_Train = ui->trainBox->currentIndex();
   QString CT_Train = ui->trainBox->currentText();
   ui->trainBox->removeItem(CI_Train);

   QString tts0 = QString("DELETE FROM Trains WHERE ID=%1").arg(CT_Train);
   INFO = db.exec(tts0);
}

void MainWindow::Load_State()
{

}

void MainWindow::Save_State()
{

}

void MainWindow::Schedule()
{

}
