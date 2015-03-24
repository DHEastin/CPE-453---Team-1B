#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Train Scheduling Application");

    rdb.addDatabase( "QMYSQL", "Remote" );
    rdb.setDatabaseName("Remote");
    rdb.setHostName("pavelow.eng.uah.edu");
    rdb.setPort(33158);
    rdb.setDatabaseName("team4b");
    rdb.setUserName("root");
    rdb.setPassword("drabroig");
    if (!rdb.open())
    {
        qDebug() << "Error connecting to Pavelow.eng.uah.edu.";
        qDebug() << "Switching database to Local Database.";
    }

    rdb.open();

    if(rdb.isOpen())
    {
       sql_query();
    }
    else
    {
        create_sqltables();
    }

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
                                         "Train", &ok);
    if (ok)
    {

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

    QString qtts0 = QString("INSERT INTO Trains (ID,Start,Direction,Destination) VALUES ('%1','%2','%3','%4')").arg(ID).arg(Start).arg(direction).arg("EMPTY");
    TRAIN = db.exec(qtts0);
    TRAIN.next();
    }
}

void MainWindow::Edit_Train()
{
    ID = ui->trainBox->currentText();

    if (ID != "")
    {

    QString VAL1,VAL2,VAL3;
    items.clear();
    QString Edit_ID = QString("SELECT ID,Start,Direction,Destination from %1 WHERE ID='%2'").arg("Trains").arg(ID);
    //QString tss4 = QString("SELECT ID,Start,Direction from %1 WHERE ID=%2").arg("Trains").arg(ID);
    TRAIN = db.exec(Edit_ID);
    TRAIN.next();
    VAL1 = TRAIN.value(0).toString();
    VAL2 = TRAIN.value(1).toString();
    VAL3 = TRAIN.value(2).toString();

    QString ts0 = QString("SELECT Current from %1").arg("DS_Connectivity");
    o = db.exec(ts0);

    /*-------------------------------------------------------------------*/
    for(;o.next() == 1;) //If it is 1 it contains data
    {
    QString sts0 = o.value(0).toString();
    items.append(sts0);
    }

    QString TITLE1 = QString("Old Start: %1 | Select NEW Starting Point for %2").arg(VAL2).arg(ID);;

    Start = QInputDialog::getItem(this, tr("Change Start"),
                                         QString(TITLE1), items, 0, false, &ok);
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

    QString TITLE2 = QString("Old Direction: %1 | Select NEW Direction for %2").arg(VAL3).arg(ID);;

    direction = QInputDialog::getItem(this, tr("Change Direction"),
                                         QString (TITLE2), items, 0, false, &ok);

    QString tts0 = QString("UPDATE Trains SET Start='%1',Direction='%2' WHERE ID='%3'").arg(Start).arg(direction).arg(ID);
    TRAIN = db.exec(tts0);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("No Train to edit!");
        msgBox.exec();
    }
}

void MainWindow::Delete_Train()
{
   int CI_Train = ui->trainBox->currentIndex();
   QString CT_Train = ui->trainBox->currentText();
   if (CT_Train != "")
   {

   QString DEL_Train = QString("DELETE FROM Trains WHERE ID='%1'").arg(CT_Train);

   QMessageBox msgBox;
   QString ID1 = QString("Delete Selected: %1?").arg(CT_Train);
   msgBox.setText(ID1);
   msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
   msgBox.setDefaultButton(QMessageBox::Ok);
   int ret = msgBox.exec();

   switch (ret) {
      case QMessageBox::Ok:
          // Ok was clicked

       ui->trainBox->removeItem(CI_Train);
       INFO = db.exec(DEL_Train);

          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
          break;
      default:
          // should never be reached
          break;
    }
   }
   else
   {
       QMessageBox msgBox;
       msgBox.setText("No Train to delete!");
       msgBox.exec();
   }
}

void MainWindow::Save_State()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable( "Trains" );
    tmodel->select();

    QString fileName = QFileDialog::getSaveFileName(this,
    tr("Save My File"), "",
    tr("My File (*.txt);;All Files (*)"));

         if (fileName.isEmpty())
             return;
         else {
             QFile file(fileName);
             if (!file.open((QIODevice::WriteOnly) | QIODevice::Text))
             {
                 QMessageBox::information(this, tr("Unable to open file"),
                     file.errorString());
                 return;
             }

             QString data;
             data ="";

          for (int row = 0; row < tmodel->rowCount(); ++row)
          {
            QSqlRecord record = tmodel->record(row);
            qDebug() << record;
                    for (int field = 0; field < record.count(); ++field)
                    {
                        if(field >= 0 && field !=5 )
                        {
                            if (field >= 0) data += "\n";
                            data += record.field(field).value().toString();
                        }
                    }
          }

          QTextStream output(&file);
          output.setCodec("UTF-8");
          output << data;
}
}

void MainWindow::Load_State()
{
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open My File"), "",
    tr("My File (*.txt);;All Files (*)"));

    if (fileName.isEmpty())
    {
    return;
    }
    else
    {
         //tmodel->clear();
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    QMessageBox::information(this, tr("Unable to open file"),
    file.errorString());
    return;
    }
    QString ttps0 = QString("DELETE FROM Trains");
    TRAIN2 = db.exec(ttps0);

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line=in.readLine(); //Move past null line
    while (!line.isNull())
        {
        QString COL1 = line=in.readLine();
        QString COL2 = line=in.readLine();
        QString COL3 = line=in.readLine();
        QString COL4 = line=in.readLine();
        QString tts0 = QString("INSERT INTO Trains (ID,Start,Direction,Destination) VALUES ('%1','%2','%3','%4')").arg(COL1).arg(COL2).arg(COL3).arg(COL4);
        TRAIN = db.exec(tts0);
        }
    QString ttps1 = QString("DELETE FROM Trains Where ID='%1'").arg("");
    TRAIN2 = db.exec(ttps1);

    QString ts1 = QString("SELECT ID from %1").arg("Trains");
    TRAIN = db.exec(ts1);
    ui->trainBox->clear();
    for(;TRAIN.next() == 1;) //If it is 1 it contains data
    {
    QString sts2 = TRAIN.value(0).toString();
    ui->trainBox->addItem(sts2);
    }

    file.close();
}
}
void MainWindow::Set_Schedule()
{
     ID = ui->trainBox->currentText();
     Destination = ui->destinationBox->currentText();
     QString ptts0 = QString("UPDATE Trains SET Destination='%1' WHERE ID='%2'").arg(Destination).arg(ID);
     TRAIN = db.exec(ptts0);
}

void MainWindow::Schedule()
{
    PATH.clear();
    dij_mainprogram();
    if (ERROR == 1)
    {
         qDebug() <<"Error, No Destination Found OR Already at Destination!";
    }
    else
    {
    qDebug() <<"Length: "<< PATH.length();

    qDebug() << "Path";
    QStringList::const_iterator constIterator;
     for (constIterator = PATH.constBegin(); constIterator != PATH.constEnd();
            ++constIterator)
         qDebug() << (*constIterator).toLocal8Bit().constData();
    }
}

void MainWindow::Train_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("Trains");
    tmodel->select();
    view->show();
}

