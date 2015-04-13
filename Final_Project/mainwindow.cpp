#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //TO DO LIST
    //---------------------------------------
    //Work on Switches
    //Work on Request interface for Team4A
    //Check Overwrite Status for Team5

    //BUGS
    //---------------------------------------
    //

    this->setWindowTitle("Train Scheduling Application");
    path_ID = 1;

    //Team 4B
    //mysql --host=pavelow.eng.uah.edu --protocol=tcp --port=33158 --user=root --password=drabroig
    //TEAM 1B
    //mysql --host=pavelow.eng.uah.edu --protocol=tcp --port=33155 --user=root --password=rychakkn
    //mysql --host=pavelow.eng.uah.edu --protocol=tcp --port=33155 --user=1BUser --password=TEAM1bUSER

    //Team1b database and user for testing
    rdb = QSqlDatabase::addDatabase( "QMYSQL" ,"Remote" );
    //rdb.addDatabase( "QMYSQL", "Remote" );
    rdb.setHostName("pavelow.eng.uah.edu");
    rdb.setPort(33155);
    rdb.setDatabaseName("Team1B");
    rdb.setUserName("1BUser");
    rdb.setPassword("TEAM1bUSER");
    /*
    +----------------------+
    | Tables_in_Team1B     |
    +----------------------+
    | schedule_train_info  |
    | scheduled_routes     |
    | scheduled_train_info |
    | switch_req           |
    | throttle_req         |
    +----------------------+
    */

    QPalette* palette = new QPalette();
    palette->setColor(QPalette::WindowText,Qt::green);
    ui->overwrite_statusLabel->setPalette(*palette);
    ui->overwrite_statusLabel->setText("Normal");

    if (!rdb.open())
    {
        QPalette* palette = new QPalette();
        palette->setColor(QPalette::WindowText,Qt::red);
        ui->connection_statusLabel->setPalette(*palette);

        ui->connection_statusLabel->setText("Disconnected!");
        ui->menuRemote_Tables->setDisabled(1);
        ui->statusBar->showMessage("Unable to connect to Pavelow.eng.uah.edu");
        qDebug() << "Error connecting to Pavelow.eng.uah.edu.";
        qDebug() << "Switching database to Local Database.";
        create_sqltables();
    }

    if(rdb.isOpen())
    {
       QPalette* palette = new QPalette();
       palette->setColor(QPalette::WindowText,Qt::green);
       ui->connection_statusLabel->setPalette(*palette);

       ui->connection_statusLabel->setText("Connected!");
       ui->menuRemote_Tables->setDisabled(0);
       //This will clean out data on Pavelow tables on startup
       QString DEL_1 = QString("DELETE FROM schedule_train_info");
       QString DEL_2 = QString("DELETE FROM scheduled_routes");
       QString DEL_3 = QString("DELETE FROM scheduled_train_info");
       r1=rdb.exec(DEL_1);
       r2=rdb.exec(DEL_2);
       r3=rdb.exec(DEL_3);

       ui->statusBar->showMessage("Connected to Pavelow.eng.uah.edu");
       qDebug() << "Connected to Pavelow.eng.uah.edu.";
       qDebug() << "Switching database to Remote Database.";
       create_sqltables();
       sql_query();
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
    connect(ui->scheduleButton,SIGNAL(clicked()),this,SLOT(Update_ScheduleTable()));
    connect(ui->actionView_Trains,SIGNAL(triggered()),this,SLOT(Train_Table()));
    connect(ui->actionView_DS_Connectivity,SIGNAL(triggered()),this,SLOT(DS_Connectivity_Table()));
    connect(ui->actionView_Tracklisting,SIGNAL(triggered()),this,SLOT(Tracklisting_Table()));
    connect(ui->actionView_Trackinfo,SIGNAL(triggered()),this,SLOT(Trackinfo_Table()));
    connect(ui->actionView_Pathinfo,SIGNAL(triggered()),this,SLOT(Pathinfo_Table()));
    connect(ui->actionView_Scheduled_routes,SIGNAL(triggered()),this,SLOT(Scheduled_routes()));
    connect(ui->actionView_Scheduled_train_info,SIGNAL(triggered()),this,SLOT(Scheduled_train_info()));
    connect(ui->actionView_Schedule_train_info,SIGNAL(triggered()),this,SLOT(Schedule_train_info()));
    connect(ui->actionView_Throttle_req,SIGNAL(triggered()),this,SLOT(Throttle_req()));
    connect(ui->actionView_Switch_req,SIGNAL(triggered()),this,SLOT(Switch_req()));
    connect(ui->actionView_Switchinfo,SIGNAL(triggered()),this,SLOT(Switch_info()));

    QTimer* persistence = new QTimer();
    QObject::connect(persistence, SIGNAL(timeout()), this, SLOT(check_sched()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*-------------------------------------------------------------------------------------------------------------*/
//Add Train allows the user to add a train to the program
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Add_Train()
{
    items.clear();
    ID = QInputDialog::getInt(this, tr("Create Train ID"),tr("TrainID:"),1, 1, 10, 1, &ok);
    if (ok)
    {

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
    QString ts1 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current='%2'").arg("DS_Connectivity").arg(Start);
    o = db.exec(ts1);
    o.next();
    QString sts1 = o.value(2).toString();
    items.append(sts1);

    //Backward
    QString sts2 = o.value(3).toString();
    items.append(sts2);

    direction = QInputDialog::getItem(this, tr("Choose Direction"),
                                         tr("Direction:"), items, 0, false, &ok);
    if (ok)
    {
        ui->trainBox->addItem(QString::number(ID));
    }

    QString qtts0 = QString("INSERT INTO Trains (ID,Start,Direction,Destination,next,pathID) VALUES ('%1','%2','%3','%4','%5','%6')").arg(ID).arg(Start).arg(direction).arg("EMPTY").arg("EMPTY").arg(999);
    o = db.exec(qtts0);
    o.next();
    if(rdb.isOpen())
    {
    QString qtts0t = QString("INSERT INTO scheduled_train_info (id,current,destination,next,pathid) VALUES ('%1','%2','%3','%4','%5')").arg(ID).arg(Start).arg("EMPTY").arg("EMPTY").arg(999);
    r1 = rdb.exec(qtts0t);
    r1.next();
    }
    }
    //scheduled_train_info
}

/*-------------------------------------------------------------------------------------------------------------*/
//Edit Train allows the user to edit the selected train's source node and facing direction
//If Trains has destination the train will reschdule itself for the new destination from the new source node
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Edit_Train()
{
    ID = ui->trainBox->currentText().toInt();

    if (ID != 0)
    {

    QString VAL1,VAL2,VAL3,VAL4;
    items.clear();
    QString Edit_ID = QString("SELECT ID,Start,Direction,Destination,pathID from %1 WHERE ID=%2").arg("Trains").arg(ID);
    TRAIN = db.exec(Edit_ID);
    TRAIN.next();
    VAL1 = TRAIN.value(0).toInt();
    VAL2 = TRAIN.value(1).toString();
    VAL3 = TRAIN.value(2).toString();
    VAL4 = TRAIN.value(3).toInt();

    QString ts0 = QString("SELECT Current from %1").arg("DS_Connectivity");
    o = db.exec(ts0);

    /*-------------------------------------------------------------------*/
    for(;o.next() == 1;) //If it is 1 it contains data
    {
    QString sts0 = o.value(0).toString();
    items.append(sts0);
    }

    QString TITLE1 = QString("Old Start: '%1' | Select NEW Starting Point for '%2'").arg(VAL2).arg(ID);;

    Start = QInputDialog::getItem(this, tr("Change Start"),
                                         QString(TITLE1), items, 0, false, &ok);
    items.clear();
    //Forward
    QString Sts1 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current='%2'").arg("DS_Connectivity").arg(Start);
    o = db.exec(Sts1);
    o.next();
    QString sts1 = o.value(2).toString();
    items.append(sts1);

    //Backward
    QString sts2 = o.value(3).toString();
    items.append(sts2);

    QString TITLE2 = QString("Old Direction: '%1' | Select NEW Direction for '%2'").arg(VAL3).arg(ID);;

    direction = QInputDialog::getItem(this, tr("Change Direction"),
                                         QString (TITLE2), items, 0, false, &ok);

    QString tts0 = QString("UPDATE Trains SET Start='%1',Direction='%2' WHERE ID='%3'").arg(Start).arg(direction).arg(ID);
    TRAIN = db.exec(tts0);
    if(rdb.isOpen())
    {
        QString tts0t = QString("UPDATE scheduled_train_info SET current='%1' WHERE ID='%2'").arg(Start).arg(ID);
        r1=rdb.exec(tts0t);
    }


    QString tqtps1 = QString("SELECT pathID FROM Trains WHERE ID='%1'").arg(ui->trainBox->currentText());
    TRAIN = db.exec(tqtps1);

    int sts22;

    for(;TRAIN.next() == 1;) //If it is 1 it contains data
    {
    sts22 = TRAIN.value(4).toInt();
    }

    QString ttps1 = QString("DELETE FROM pathInfoTable WHERE pathID=%1").arg(sts22);
    TRAIN2 = db.exec(ttps1);

    if(rdb.isOpen())
    {
        QString DEL_1 = QString("DELETE FROM scheduled_routes WHERE id=%1").arg(ui->trainBox->currentText());
        r1=rdb.exec(DEL_1);
    }

    QString ttps2 = QString("DELETE FROM pathInfoTable WHERE nextID1='%1'").arg("NULL");
    TRAIN2 = db.exec(ttps2);

    if(rdb.isOpen())
    {
        QString DEL_1 = QString("DELETE FROM scheduled_routes WHERE id=%1").arg(ui->trainBox->currentText());
        r2=rdb.exec(DEL_1);
    }

    DEL_OLD_PATH();
    Check_Path_Trains();
    if(ui->rescheduleBox->isChecked())
    {
    Schedule();
    Update_ScheduleTable();
    }

    tmodel = new QSqlTableModel( this, db );
    ui->tableView->setModel(tmodel);
    tmodel->setTable("pathInfoTable");
    tmodel->select();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Train Error!");
        msgBox.setText("No Train to edit!");
        QSpacerItem* horizontalSpacer = new QSpacerItem(200, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
        msgBox.exec();
    }
}

/*-------------------------------------------------------------------------------------------------------------*/
//Delete Train deletes the selected train and removes the train from the scheduler
/*-------------------------------------------------------------------------------------------------------------*/

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
   QSpacerItem* horizontalSpacer = new QSpacerItem(200, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
   QGridLayout* layout = (QGridLayout*)msgBox.layout();
   layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
   msgBox.setWindowTitle("Delete Train");
   msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
   msgBox.setDefaultButton(QMessageBox::Ok);
   int ret = msgBox.exec();

   switch (ret) {
      case QMessageBox::Ok:
          // Ok was clicked
   {

       DEL_OLD_PATH();
       Check_Path_Trains();

       ui->trainBox->removeItem(CI_Train);
       INFO = db.exec(DEL_Train);

       if(rdb.isOpen())
       {
           QString DEL_1 = QString("DELETE FROM scheduled_train_info WHERE id=%1").arg(CT_Train);
           r1=rdb.exec(DEL_1);
       }

       tmodel = new QSqlTableModel( this, db );
       ui->tableView->setModel(tmodel);
       tmodel->setTable("pathInfoTable");
       tmodel->select();
   }

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
       msgBox.setWindowTitle("Train Error!");
       msgBox.setText("No Train to delete!");
       QSpacerItem* horizontalSpacer = new QSpacerItem(200, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
       QGridLayout* layout = (QGridLayout*)msgBox.layout();
       layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
       msgBox.exec();
   }
}

/*-------------------------------------------------------------------------------------------------------------*/
//Save_State saves the Train & Pathinfo Table to a text file for later restoring using Load_State
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Save_State()
{
    tmodel = new QSqlTableModel( this, db );
    tmodel2 = new QSqlTableModel( this, db );
    view = new QTableView;
    view2 = new QTableView;
    view->setModel(tmodel);
    view2->setModel(tmodel2);
    tmodel->setTable( "Trains" );
    tmodel->select();
    tmodel2->setTable( "pathInfoTable" );
    tmodel2->select();

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
            //qDebug() <<"rowCount()= "<< tmodel->rowCount();
              //qDebug() << row;
            QSqlRecord record = tmodel->record(row);
            //qDebug() << record;
                    for (int field = 0; field < record.count(); ++field)
                    {
                        if(field >= 0 && field !=6 )
                        {
                            if (field >= 0) data += "\n";
                            data += record.field(field).value().toString();
                        }                        
                    }
                    int row_count = tmodel->rowCount() - 1;
                    if(row != row_count)
                    {
                    data += "\n";
                    }
          }

           data += "\npathInfoTable";

          for (int row = 0; row < tmodel2->rowCount(); ++row)
          {
            //qDebug() <<"2rowCount()= "<< tmodel2->rowCount();
            //qDebug() << row;
            QSqlRecord record = tmodel2->record(row);
            //qDebug() << record;

                    for (int field = 0; field < record.count(); ++field)
                    {
                        if(field >= 0 && field !=13 )
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

/*-------------------------------------------------------------------------------------------------------------*/
//Load_State restores the text file from Save_State restoring trains and paths stored
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Load_State()
{
    bool SKIP = 0;

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
        if(SKIP != 1)
        {
            //qDebug() << "SKIP !=1";
            QString COL1 = line=in.readLine();
            QString COL2 = line=in.readLine();
            QString COL3 = line=in.readLine();
            QString COL4 = line=in.readLine();
            QString COL5 = line=in.readLine();
        if(COL1 != "pathInfoTable" && COL2 != "pathInfoTable" && COL3 != "pathInfoTable" && COL4 != "pathInfoTable" && COL5 != "pathInfoTable")
        {
        QString tts0 = QString("INSERT INTO Trains (ID,Start,Direction,Destination,pathID) VALUES ('%1','%2','%3','%4',%5)").arg(COL1).arg(COL2).arg(COL3).arg(COL4).arg(COL5);
        TRAIN = db.exec(tts0);
        }
        QString NEXT = line=in.readLine();
        if(NEXT == "pathInfoTable")
        {
            //qDebug() << "Set SKIP to 1";
            SKIP = 1;
        }
        }
        else
        {
            //qDebug() << "SKIP ==1";
            QString COL1 = line=in.readLine();
            QString COL2 = line=in.readLine();
            QString COL3 = line=in.readLine();
            QString COL4 = line=in.readLine();
            QString COL5 = line=in.readLine();
            QString COL6 = line=in.readLine();
            QString COL7 = line=in.readLine();
            QString COL8 = line=in.readLine();
            QString COL9 = line=in.readLine();
            QString COL10 = line=in.readLine();
            QString COL11 = line=in.readLine();
            QString COL12 = line=in.readLine();

             QString path = QString("INSERT INTO pathInfoTable (pathID,nextID1,nextID2,nextID3,nextID4,nextID5,nextID6,nextID7,nextID8,nextID9,nextID10,nextpathID) VALUES (%1,'%2','%3','%4','%5','%6','%7','%8','%9','%10','%11',%12)").arg(COL1).arg(COL2).arg(COL3).arg(COL4).arg(COL5).arg(COL6).arg(COL7).arg(COL8).arg(COL9).arg(COL10).arg(COL11).arg(COL12);
             Path = db.exec(path);
        }
        }//end while loop

    tmodel = new QSqlTableModel( this, db );
    ui->tableView->setModel(tmodel);
    tmodel->setTable("pathInfoTable");
    tmodel->select();

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

/*-------------------------------------------------------------------------------------------------------------*/
//Set_Schedule updates the train table for the new destination when Schedule button is pressed
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Set_Schedule()
{
     ID = ui->trainBox->currentText().toInt();
     Destination = ui->destinationBox->currentText();
     //current
     QString ptts0 = QString("UPDATE Trains SET Destination='%1' WHERE ID='%2'").arg(Destination).arg(ID);
     TRAIN = db.exec(ptts0);
     if(rdb.isOpen())
     {
         QString ptts0t = QString("UPDATE scheduled_train_info SET destination='%1' WHERE ID='%2'").arg(Destination).arg(ID);
         r1=rdb.exec(ptts0t);
     }
}

/*-------------------------------------------------------------------------------------------------------------*/
//Schedule() schedules the train from the source node to the destination node
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Schedule()
{
    Set_Schedule();

    PATH.clear();
    dij_mainprogram();
    if (ERROR == 1)
    {
         qDebug() <<"Error, No Destination Found OR Already at Destination!";
         DEL_OLD_PATH();
    }
    else
    {
    //DEL_OLD_PATH();
    qDebug() <<"Length: "<< PATH.length();

    qDebug() << "Path";
    QStringList::const_iterator constIterator;
     for (constIterator = PATH.constBegin(); constIterator != PATH.constEnd();
            ++constIterator)
         qDebug() << (*constIterator).toLocal8Bit().constData();
    }
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of Trains Table
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Train_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("Trains");
    tmodel->select();
    view->setWindowTitle("Trains");
    view->show();
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of DS_Connectivity Table
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::DS_Connectivity_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("DS_Connectivity");
    tmodel->select();
    view->setWindowTitle("DS_Connectivity");
    view->show();
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of tracklisting Table
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Tracklisting_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("tracklistingTable");
    tmodel->select();
    view->setWindowTitle("tracklistingTable");
    view->show();
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of trackinfo Table
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Trackinfo_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("trackInfoTable");
    tmodel->select();
    view->setWindowTitle("trackInfoTable");
    view->show();
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of pathinfo Table
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Pathinfo_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("pathInfoTable");
    tmodel->select();
    view->setWindowTitle("pathInfoTable");
    view->show();
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of switchinfo Table
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Switch_info()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("switchInfoTable");
    tmodel->select();
    view->setWindowTitle("switchInfoTable");
    view->show();
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of Pavelow schedule_train_info Table
/*-------------------------------------------------------------------------------------------------------------*/
void MainWindow::Schedule_train_info()
{
    if(rdb.isOpen())
    {
    tmodel = new QSqlTableModel( this, rdb );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("schedule_train_info");
    tmodel->select();
    view->setWindowTitle("schedule_train_info");
    view->show();
    }
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of Pavelow Throttle_req Table
/*-------------------------------------------------------------------------------------------------------------*/
void MainWindow::Throttle_req()
{
    if(rdb.isOpen())
    {
    tmodel = new QSqlTableModel( this, rdb );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("throttle_req");
    tmodel->select();
    view->setWindowTitle("throttle_req");
    view->show();
    }
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of Pavelow Switch_req Table
/*-------------------------------------------------------------------------------------------------------------*/
void MainWindow::Switch_req()
{
    if(rdb.isOpen())
    {
    tmodel = new QSqlTableModel( this, rdb );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("switch_req");
    tmodel->select();
    view->setWindowTitle("switch_req");
    view->show();
    }
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of Pavelow scheduled_routes Table
/*-------------------------------------------------------------------------------------------------------------*/
void MainWindow::Scheduled_routes()
{
    if(rdb.isOpen())
    {
    tmodel = new QSqlTableModel( this, rdb );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("scheduled_routes");
    tmodel->select();
    view->setWindowTitle("scheduled_routes");
    view->show();
    }
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of Pavelow scheduled_train_info Table
/*-------------------------------------------------------------------------------------------------------------*/
void MainWindow::Scheduled_train_info()
{
    if(rdb.isOpen())
    {
    tmodel = new QSqlTableModel( this, rdb );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("scheduled_train_info");
    tmodel->select();
    view->setWindowTitle("scheduled_train_info");
    view->show();
    }
}

/*-------------------------------------------------------------------------------------------------------------*/
//Update_ScheduleTable updates the pathinfo table with the path determined by scheduler
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Update_ScheduleTable()
{
    PATH.begin();
    if(ERROR == 1)
    {

    }
    else
    {
    qDebug() <<"PATH Length: "<< PATH.length();
    Check_Path_Trains();
    DEL_OLD_PATH();
    if (PATH.length() <= 11) // Equal or Less than PATH.length()
    {
    int LENL = 0;
    int LEN;
    //qDebug() <<"LEN= "<< LEN <<"PATH.length()= "<<PATH.length();
    QString qtts0 = "INSERT INTO pathInfoTable (pathID,nextID1,nextID2,nextID3,nextID4,nextID5,nextID6,nextID7,nextID8,nextID9,nextID10,nextpathID) VALUES (";
    QString qtts0tt = "INSERT INTO scheduled_routes (pathid,next2,next3,next4,next5,next6,next7,next8,next9,next10,next11,nextpath) VALUES (";
    for (LEN = 0;LEN != PATH.length();LEN++ )
    {
        if(LENL == 0)
        {
            QString I_D;
            I_D = QString("%1").arg(path_ID);
            qtts0.append(I_D);
            qtts0.append(",");
            qtts0tt.append(I_D);
            qtts0tt.append(",");
        }
        else
        {
            QString I_D;
            I_D = QString("'%1'").arg(PATH.value(LEN));
            qtts0.append(I_D);
            qtts0.append(",");
            qtts0tt.append(I_D);
            qtts0tt.append(",");
        }
        LENL++;
    }

    //qDebug() <<"LEN= "<< LEN;
        for (int C_ID = LEN;C_ID != 11; C_ID++)
        {
            //qDebug() <<"C_ID= "<< C_ID;
            qtts0.append("'NULL'");
            qtts0.append(",");
            qtts0tt.append("'NULL'");
            qtts0tt.append(",");
        }
    QString I_D;
    I_D = QString("%1").arg(path_ID);
    qtts0.append(I_D);
    qtts0.append(")");
    qtts0tt.append(I_D);
    qtts0tt.append(")");
    //qDebug() << qtts0;

    o = db.exec(qtts0);
    if(rdb.isOpen())
    {
    r3 = rdb.exec(qtts0tt);
    }

        QString train_up = QString("UPDATE Trains SET pathID=%1 WHERE ID='%2'").arg(path_ID).arg(ui->trainBox->currentText());
        TRAIN = db.exec(train_up);

        if(rdb.isOpen())
        {
        QString train_up2 = QString("UPDATE scheduled_train_info SET pathid=%1 WHERE id='%2'").arg(path_ID).arg(ui->trainBox->currentText());
        r2 = rdb.exec(train_up2);
        }
    }
    else //Greater than 10 points between source and destination (Multi-line Path)
    {
        bool ok = 0;
        int tot,tot2;
        //Must use # of paths for # of full rows of data *each row is 10 nodes

        int LEN = 0;
        int TOT = PATH.length();
        int TOT_LEFT = TOT - LEN;
        if (TOT > 23)
        {
            ok = 1;
            QString qtts0 = "INSERT INTO pathInfoTable (pathID,nextID1,nextID2,nextID3,nextID4,nextID5,nextID6,nextID7,nextID8,nextID9,nextID10,nextpathID) VALUES (";
            QString qtts0tq = "INSERT INTO scheduled_routes (pathid,next2,next3,next4,next5,next6,next7,next8,next9,next10,next11,nextpath) VALUES (";
            for (LEN= LEN;LEN != 11;LEN++)
            {
            if(LEN == 0)
            {
                QString I_D;
                I_D = QString("%1").arg(path_ID);
                qtts0.append(I_D);
                qtts0.append(",");
                qtts0tq.append(I_D);
                qtts0tq.append(",");
            }

            else
            {
                QString I_D;
                I_D = QString("'%1'").arg(PATH.value(LEN));
                qtts0.append(I_D);
                qtts0.append(",");
                qtts0tq.append(I_D);
                qtts0tq.append(",");
            }
            }
            QString I_D;
            I_D = QString("%1").arg(path_ID++);
            qtts0.append(I_D);
            qtts0.append(")");
            qtts0tq.append(I_D);
            qtts0tq.append(",");
            //qDebug() << qtts0;

            o = db.exec(qtts0);
            if(rdb.isOpen())
            {
            r3 = rdb.exec(qtts0tq);
            }
        }
            if(ok == 1)
            {
                tot = 22;
                tot2 = 33;
            }
            else
            {
                tot = 11;
                tot2 = 22;
            }
            int LEFT = TOT_LEFT - tot;
            QString qtts0 = "INSERT INTO pathInfoTable (pathID,nextID1,nextID2,nextID3,nextID4,nextID5,nextID6,nextID7,nextID8,nextID9,nextID10,nextpathID) VALUES (";
            QString qtts0tw = "INSERT INTO scheduled_routes (pathid,next2,next3,next4,next5,next6,next7,next8,next9,next10,next11,nextpath) VALUES (";
            for (LEN= LEN;LEN != tot;LEN++)
            {
            if(LEN == 0)
            {
                QString I_D;
                I_D = QString("%1").arg(path_ID);
                qtts0.append(I_D);
                qtts0.append(",");
                qtts0tw.append(I_D);
                qtts0tw.append(",");

                QString train_up = QString("UPDATE Trains SET pathID=%1 WHERE ID='%2'").arg(path_ID).arg(ui->trainBox->currentText());
                TRAIN = db.exec(train_up);
                if(rdb.isOpen())
                {
                QString train_up2 = QString("UPDATE scheduled_train_info SET pathid=%1 WHERE id='%2'").arg(path_ID).arg(ui->trainBox->currentText());
                r2 = rdb.exec(train_up2);
                }
            }

            else
            {
                QString I_D;
                I_D = QString("'%1'").arg(PATH.value(LEN));
                qtts0.append(I_D);
                qtts0.append(",");
                qtts0tw.append(I_D);
                qtts0tw.append(",");
            }
            }
            QString I_D;
            path_ID++;
            I_D = QString("%1").arg(path_ID);
            qtts0.append(I_D);
            qtts0.append(")");
            qtts0tw.append(I_D);
            qtts0tw.append(")");
            //qDebug() << qtts0;

            o = db.exec(qtts0);
            if(rdb.isOpen())
            {
            r3 = rdb.exec(qtts0tw);
            }

        //qDebug() <<"LEFT= "<< LEFT;
        if(LEFT <= 11) //Less than 10 points between source and destination
        {
            int LENL = 0;
            //qDebug() <<"LEN= "<< LEN <<"PATH.length()= "<<PATH.length();
            QString qtts0 = "INSERT INTO pathInfoTable (pathID,nextID1,nextID2,nextID3,nextID4,nextID5,nextID6,nextID7,nextID8,nextID9,nextID10,nextpathID) VALUES (";
            QString qtts0te = "INSERT INTO scheduled_routes (pathid,next2,next3,next4,next5,next6,next7,next8,next9,next10,next11,nextpath) VALUES (";
            for (LEN = LEN;LEN != PATH.length();LEN++ )
            {
                if(LENL == 0)
                {
                    QString I_D;
                    I_D = QString("%1").arg(path_ID);
                    qtts0.append(I_D);
                    qtts0.append(",");
                    qtts0te.append(I_D);
                    qtts0te.append(",");
                }
                else
                {
                    QString I_D;
                    I_D = QString("'%1'").arg(PATH.value(LEN));
                    qtts0.append(I_D);
                    qtts0.append(",");
                    qtts0te.append(I_D);
                    qtts0te.append(",");
                }
                LENL++;
            }

            //qDebug() <<"LEN= "<< LEN;
                for (int C_ID = LEN;C_ID != tot2; C_ID++)
                {
                    //qDebug() <<"C_ID= "<< C_ID;
                    qtts0.append("'NULL'");
                    qtts0.append(",");
                    qtts0te.append("'NULL'");
                    qtts0te.append(",");
                }
            QString I_D;
            I_D = QString("%1").arg(path_ID);
            qtts0.append(I_D);
            qtts0.append(")");
            qtts0te.append(I_D);
            qtts0te.append(")");
            //qDebug() << qtts0;

            o = db.exec(qtts0);
            if(rdb.isOpen())
            {
            r3 = rdb.exec(qtts0te);
            }
    }
}
    tmodel = new QSqlTableModel( this, db );
    ui->tableView->setModel(tmodel);
    tmodel->setTable("pathInfoTable");
    tmodel->select();
    //view->show();
    path_ID++;
}
}

/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::DEL_OLD_PATH()
{
    int sts1/*,sts2*/,sts3,sts4/*,sts5*/;

    QString string_s1;

    QString tqtps1 = QString("SELECT pathID FROM Trains WHERE ID='%1'").arg(ui->trainBox->currentText());
    TRAIN = db.exec(tqtps1);

    for(;TRAIN.next() == 1;) //If it is 1 it contains data
    {
    sts1 = TRAIN.value(4).toInt();
    }

    int Test = sts1+1;

    //qDebug() <<sts1<<Test;

    QString tqtps2 = QString("SELECT pathID,nextpathID FROM pathInfoTable WHERE pathID=%1").arg(sts1);
    Path = db.exec(tqtps2);

    for(;Path.next() == 1;) //If it is 1 it contains data
    {
    //sts2 = Path.value(0).toInt();
    sts3 = Path.value(1).toInt();
    string_s1 = Path.value(2).toString();
    }

    QString tqtps3 = QString("SELECT pathID,nextID1,nextpathID FROM pathInfoTable WHERE pathID=%1").arg(Test);
    Path = db.exec(tqtps3);

    for(;Path.next() == 1;) //If it is 1 it contains data
    {
    sts4 = Path.value(0).toInt();
    //string_s1 = Path.value(1).toString();
    }

    if(string_s1=="")
    {
        QString ttps2 = QString("DELETE FROM pathInfoTable WHERE nextID1='%1'").arg("NULL");
        TRAIN2 = db.exec(ttps2);

        if(rdb.isOpen())
        {
        QString qtts0t = QString("DELETE FROM scheduled_routes WHERE next2='%1'").arg("NULL");
        r2 = rdb.exec(qtts0t);
        }
    }

    //qDebug() << sts1<< sts2<< sts3<<sts4<<sts5;

    QString ttps1 = QString("DELETE FROM pathInfoTable WHERE pathID=%1").arg(sts1);
    TRAIN2 = db.exec(ttps1);

    if(rdb.isOpen())
    {
    QString qtts0t = QString("DELETE FROM scheduled_routes WHERE pathid='%1'").arg(sts1);
    r2 = rdb.exec(qtts0t);
    }

    if(sts3 == sts4)
    {
        QString ttps1 = QString("DELETE FROM pathInfoTable WHERE pathID=%1").arg(sts4);
        Path2 = db.exec(ttps1);

        if(rdb.isOpen())
        {
        QString qtts0t = QString("DELETE FROM scheduled_routes WHERE pathid=%1").arg(sts4);
        r2 = rdb.exec(qtts0t);
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------*/
//Deletes unused/deleted train paths from pathinfoTable
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Check_Path_Trains()
{
    int sts1=0;
    int sts2=0;
    int sts3=0;

    QString tqtps1 = QString("SELECT pathID FROM Trains WHERE ID='%1'").arg(ui->trainBox->currentText());
    TRAIN = db.exec(tqtps1);

    for(;TRAIN.next() == 1;) //If it is 1 it contains data
    {
    sts1 = TRAIN.value(0).toInt();
    }

    int Sts1 = sts1 + 1;
    QString string_ts;

    QString tqtps2 = QString("SELECT pathID,nextID1 FROM pathInfoTable WHERE pathID=%1").arg(sts1);
    Path = db.exec(tqtps2);

    for(;Path.next() == 1;) //If it is 1 it contains data
    {
    sts2 = Path.value(0).toInt();
    string_ts = Path.value(1).toString();
    }

    QString tqtps3 = QString("SELECT pathID FROM pathInfoTable WHERE pathID=%1").arg(Sts1);
    Path = db.exec(tqtps3);

    for(;Path.next() == 1;) //If it is 1 it contains data
    {
    sts3 = Path.value(0).toInt();
    }

    //qDebug() <<"sts1= "<<sts1<<"sts2= "<<sts2<<"sts3= "<<sts3;

    if(sts1 == sts2)
    {
        QString ttps1 = QString("DELETE FROM pathInfoTable WHERE pathID=%1").arg(sts1);
        Path = db.exec(ttps1);
        if(rdb.isOpen())
        {
        QString qtts0t = QString("DELETE FROM scheduled_routes WHERE pathID=%1").arg(sts1);
        r1 = rdb.exec(qtts0t);
        }
    }

    if(Sts1 == sts3)
    {
        QString ttps1 = QString("DELETE FROM pathInfoTable WHERE pathID=%1").arg(sts3);
        Path2 = db.exec(ttps1);
        if(rdb.isOpen())
        {
        QString qtts0t = QString("DELETE FROM scheduled_routes WHERE pathID=%1'").arg(sts3);
        r2 = rdb.exec(qtts0t);
        }
    }

    //qDebug() <<"string_ts= "<<string_ts;

    if(string_ts == "")
    {
        QString ttps1 = QString("DELETE FROM pathInfoTable WHERE nextID1='%1'").arg("NULL");
        Path2 = db.exec(ttps1);
        if(rdb.isOpen())
        {
        QString qtts0t = QString("DELETE FROM scheduled_routes WHERE next2='%1'").arg("NULL");
        r3 = rdb.exec(qtts0t);
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------*/
//check_sched will update Pavelow Team 3 Table with desired path
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::check_sched()
{
    if(rdb.isOpen())
    {

    tmodel = new QSqlTableModel( this, rdb );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("pathInfoTable");
    tmodel->select();
    view->show();

    }

//check team 2 SQL for track shutdowns. If any new ones, check schedules, trigger reroutes if necessary

//check occupancy data and adjust throttles accordingly,
    //If a schedule cannot be made up with throttle adjustments, trigger reroute
}
