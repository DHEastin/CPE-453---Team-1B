#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //TO DO LIST
    //---------------------------------------
    //Implement checking for direction by checking nextID1 compared to direction -- if no path found -- MSGBOX FLIP TRAIN

    //BUGS
    //---------------------------------------
    //When train has reached destination:
    //  PATHID does not change back to 999
    //  Old Path is not deleted from Path Table
    //Issues with path moving when dealing with multiple row paths
    //When adding a train prevent multiple trains with same ID from showing up in TrainBox

    //OPTIONS
    Testing_path = 0; //0 -- Path will not change unless connected to Pavelow  //1 -- Path will update even while disconnected from pavelow

    this->setWindowTitle("Train Scheduling Application");
    path_ID = 1;

    //Team 4A
    //mysql --host=pavelow.eng.uah.edu --protocol=tcp --port=33153 --user=root --password=cstrapwi
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

    //Team1A for interoperability
    /*
    rdb = QSqlDatabase::addDatabase( "QMYSQL" ,"Remote" );
    //rdb.addDatabase( "QMYSQL", "Remote" );
    rdb.setHostName("pavelow.eng.uah.edu");
    rdb.setPort(33153);
    rdb.setDatabaseName("cpe453");
    rdb.setUserName("root");
    rdb.setPassword("cstrapwi");
    */


    //override_status
    //st    mode    estop
    overrideStatus = false;

    QPalette* palette = new QPalette();
    palette->setColor(QPalette::WindowText,Qt::blue);
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
       palette->setColor(QPalette::WindowText,Qt::blue);
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
    QString s9 = QString("SELECT switch,position,straight,openPOS,closedPOS from %3").arg("switchInfoTable");
    p = db.exec(s9);

    for(;p.next() == 1;) //Check switchInfoTable table for data
    {
        int COL1 = p.value(0).toInt();
        int COL2 = p.value(4).toInt();

        QString smpms = QString("INSERT INTO trackInfoTable (currentnode, nextnode, weight) VALUES (%1,%2,5);").arg(COL1).arg(COL2);
        qq.exec(smpms);

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
    connect(ui->actionView_Trackinfo2,SIGNAL(triggered()),this,SLOT(Trackinfo2_Table()));
    connect(ui->actionView_Pathinfo,SIGNAL(triggered()),this,SLOT(Pathinfo_Table()));
    connect(ui->actionView_Scheduled_routes,SIGNAL(triggered()),this,SLOT(Scheduled_routes()));
    connect(ui->actionView_Scheduled_train_info,SIGNAL(triggered()),this,SLOT(Scheduled_train_info()));
    connect(ui->actionView_Schedule_train_info,SIGNAL(triggered()),this,SLOT(Schedule_train_info()));
    connect(ui->actionView_Throttle_req,SIGNAL(triggered()),this,SLOT(Throttle_req()));
    connect(ui->actionView_Switch_req,SIGNAL(triggered()),this,SLOT(Switch_req()));
    connect(ui->actionView_Switchinfo,SIGNAL(triggered()),this,SLOT(Switch_info()));
    connect(ui->actionView_Trains1,SIGNAL(triggered()),this,SLOT(Trains1_Table()));
    connect(ui->actionView_Trains2,SIGNAL(triggered()),this,SLOT(Trains2_Table()));
    connect(ui->actionView_Trains3,SIGNAL(triggered()),this,SLOT(Trains3_Table()));
    connect(ui->actionView_Trains4,SIGNAL(triggered()),this,SLOT(Trains4_Table()));
    connect(ui->actionView_Trains5,SIGNAL(triggered()),this,SLOT(Trains5_Table()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(check_sched()));
    timer->start(500);
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
    trains.clear();
    ID = QInputDialog::getInt(this, tr("Input Train Address"),tr("TrainID:"),1, 1, 5, 1, &ok);
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
    QString sts1 = o.value(2).toString();//Forward -- Counter Clockwise
    items.append(sts1);

    //Backward
    QString sts2 = o.value(3).toString();//Backward -- Clockwise
    items.append(sts2);

    direction = QInputDialog::getItem(this, tr("Choose Direction"),
                                         tr("Direction:"), items, 0, false, &ok);
    if (ok)
    {
        ui->trainBox->addItem(QString::number(ID));

        if(direction == sts1)
        {
            CLOCK = "CC";
        }
        if(direction == sts2)
        {
            CLOCK = "C";
        }
    }

    QString qtts0 = QString("INSERT INTO Trains (ID,Start,Direction,Destination,next,pathID,DIR) VALUES ('%1','%2','%3','%4','%5','%6','%7')").arg(ID).arg(Start).arg(direction).arg("EMPTY").arg("EMPTY").arg(999).arg(CLOCK);
    o = db.exec(qtts0);
    o.next();
    if(rdb.isOpen())
    {
    QString qtts0t = QString("INSERT INTO scheduled_train_info (id,current,destination,next,pathid) VALUES ('%1','%2','%3','%4','%5')").arg(ID).arg(Start).arg("EMPTY").arg("EMPTY").arg(999);
    r1 = rdb.exec(qtts0t);
    r1.next();
    }
    }
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

    if(direction == sts1)
    {
        CLOCK = "CC";
    }
    if(direction == sts2)
    {
        CLOCK = "C";
    }

    QString tts0 = QString("UPDATE Trains SET Start='%1',Direction='%2',DIR='%3' WHERE ID='%4'").arg(Start).arg(direction).arg(CLOCK).arg(ID);
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
    DEL_ALL_OLD_PATH();
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


     //Moved
     QString stqs1,stqs2;
     int sts1,sts2;
     QString tqtps1 = QString("SELECT ID,next,pathID FROM Trains");
     QString smi3 = QString("SELECT pathID,nextID1 from %1").arg("pathInfoTable");
     Path2 = db.exec(smi3);
     TRAIN = db.exec(tqtps1);

     //Check each train
     for(;TRAIN.next() == 1;) //If it is 1 it contains data
     {
         sts2 = TRAIN.value(2).toInt();
         //stqs1 = TRAIN.value(0).toString();
         for(;Path2.next() == 1;)
         {
             sts1 = Path2.value(0).toInt();
             stqs1 = Path2.value(1).toString();
             if(sts1 == sts2)
             {
                 stqs2 = stqs1;
             }
         }
         QString train_up2 = QString("UPDATE Trains SET next='%1' WHERE pathID=%2").arg(stqs2).arg(sts2);
         //TRAIN = db.exec(train_up2);
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
//Allows viewing of trackinfo Table
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Trackinfo2_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("trackInfoTable2");
    tmodel->select();
    view->setWindowTitle("trackInfoTable2");
    view->show();
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of Trains1 Table
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Trains1_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("Trains1");
    tmodel->select();
    view->setWindowTitle("Trains1");
    view->show();
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of Trains2 Table
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Trains2_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("Trains2");
    tmodel->select();
    view->setWindowTitle("Trains2");
    view->show();
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of Trains3 Table
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Trains3_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("Trains3");
    tmodel->select();
    view->setWindowTitle("Trains3");
    view->show();
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of Trains4 Table
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Trains4_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("Trains4");
    tmodel->select();
    view->setWindowTitle("Trains4");
    view->show();
}

/*-------------------------------------------------------------------------------------------------------------*/
//Allows viewing of Trains5 Table
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::Trains5_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("Trains5");
    tmodel->select();
    view->setWindowTitle("Trains5");
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
    if(ui->trainBox->currentText() == "1")
    {
        QString SW_IT1 = QString("DELETE FROM Trains1");
        TrainSW1.exec(SW_IT1);
    }
    if(ui->trainBox->currentText() == "2")
    {
        QString SW_IT2 = QString("DELETE FROM Trains2");
        TrainSW2.exec(SW_IT2);
    }
    if(ui->trainBox->currentText() == "3")
    {
        QString SW_IT3 = QString("DELETE FROM Trains3");
        TrainSW3.exec(SW_IT3);
    }
    if(ui->trainBox->currentText() == "4")
    {
        QString SW_IT4 = QString("DELETE FROM Trains4");
        TrainSW4.exec(SW_IT4);
    }
    if(ui->trainBox->currentText() == "5")
    {
        QString SW_IT5 = QString("DELETE FROM Trains5");
        TrainSW5.exec(SW_IT5);
    }

    //Update Personal Track Table with switches that need to be switches for that train's path
    PATH.begin();
    int tt = 0;
    for (tt = tt; tt != PATH.length(); tt++)
    {
        QString PATH_VALUE;
        int sts0,sts1,sts2,sts3,SWITCH;

        PATH_VALUE = PATH.value(tt);

        //Convert String PATH_VALUE to tracklisting int Value
        QString tqtps1 = QString("SELECT trackID FROM tracklistingTable WHERE trackNAME='%1'").arg(PATH_VALUE);
        n = db.exec(tqtps1);

        n.next();
        sts0 = n.value(0).toInt(); //trackID

        //Check each switch along PATH
        QString tqtps2 = QString("SELECT switch,openPOS,closedPOS FROM switchInfoTable WHERE closedPOS=%1").arg(sts0);
        p = db.exec(tqtps2);

        for(;p.next() == 1;)
        {
        sts1 = p.value(0).toInt();
        sts2 = p.value(1).toInt(); //Open Position
        sts3 = p.value(2).toInt(); //Closed Position

        if(sts0 == sts3)
        {
            SWITCH = 1;
        }
        else
        {
            SWITCH = 0;
        }

        if(ui->trainBox->currentText() == "1")
        {
            QString SW_IT4 = QString("INSERT INTO Trains1 (switch, position) VALUES (%1,%2);").arg(sts1).arg(SWITCH);
            TrainSW1.exec(SW_IT4);
        }
        if(ui->trainBox->currentText() == "2")
        {
            QString SW_IT5 = QString("INSERT INTO Trains2 (switch, position) VALUES (%1,%2);").arg(sts1).arg(SWITCH);
            TrainSW2.exec(SW_IT5);
        }
        if(ui->trainBox->currentText() == "3")
        {
            QString SW_IT6 = QString("INSERT INTO Trains3 (switch, position) VALUES (%1,%2);").arg(sts1).arg(SWITCH);
            TrainSW3.exec(SW_IT6);
        }
        if(ui->trainBox->currentText() == "4")
        {
            QString SW_IT7 = QString("INSERT INTO Trains4 (switch, position) VALUES (%1,%2);").arg(sts1).arg(SWITCH);
            TrainSW4.exec(SW_IT7);
        }
        if(ui->trainBox->currentText() == "5")
        {
            QString SW_IT8 = QString("INSERT INTO Trains5 (switch, position) VALUES (%1,%2);").arg(sts1).arg(SWITCH);
            TrainSW5.exec(SW_IT8);
        }
        }

        QString tqtps3 = QString("SELECT switch,openPOS,closedPOS FROM switchInfoTable WHERE openPOS=%1").arg(sts0);
        p = db.exec(tqtps3);

        for(;p.next() == 1;)
        {
        sts1 = p.value(0).toInt();
        sts2 = p.value(1).toInt(); //Open Position
        sts3 = p.value(2).toInt(); //Closed Position

        if(sts0 == sts3)
        {
            SWITCH = 1;
        }
        else
        {
            SWITCH = 0;
        }

        if(ui->trainBox->currentText() == "1")
        {
            QString SW_IT4 = QString("INSERT INTO Trains1 (switch, position) VALUES (%1,%2);").arg(sts1).arg(SWITCH);
            TrainSW1.exec(SW_IT4);
        }
        if(ui->trainBox->currentText() == "2")
        {
            QString SW_IT5 = QString("INSERT INTO Trains2 (switch, position) VALUES (%1,%2);").arg(sts1).arg(SWITCH);
            TrainSW2.exec(SW_IT5);
        }
        if(ui->trainBox->currentText() == "3")
        {
            QString SW_IT6 = QString("INSERT INTO Trains3 (switch, position) VALUES (%1,%2);").arg(sts1).arg(SWITCH);
            TrainSW3.exec(SW_IT6);
        }
        if(ui->trainBox->currentText() == "4")
        {
            QString SW_IT7 = QString("INSERT INTO Trains4 (switch, position) VALUES (%1,%2);").arg(sts1).arg(SWITCH);
            TrainSW4.exec(SW_IT7);
        }
        if(ui->trainBox->currentText() == "5")
        {
            QString SW_IT8 = QString("INSERT INTO Trains5 (switch, position) VALUES (%1,%2);").arg(sts1).arg(SWITCH);
            TrainSW5.exec(SW_IT8);
        }
        }      
        }

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
            qtts0.append("NULL");
            qtts0.append(",");
            qtts0tt.append("NULL");
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
        if (TOT >= 23)
        {
            ok = 1;
            tot = 11;
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
        }
            if(ok == 1)
            {
                tot = 22;
                tot2 = 32;
            }
            else
            {
                tot = 11;
                tot2 = 21;
            }
            int LEFT = TOT_LEFT - tot;
            QString qtts01 = "INSERT INTO pathInfoTable (pathID,nextID1,nextID2,nextID3,nextID4,nextID5,nextID6,nextID7,nextID8,nextID9,nextID10,nextpathID) VALUES (";
            QString qtts0tw = "INSERT INTO scheduled_routes (pathid,next2,next3,next4,next5,next6,next7,next8,next9,next10,next11,nextpath) VALUES (";
            for (LEN= LEN;LEN != tot;LEN++)
            {
            if(LEN == 0)
            {
                QString I_D;
                I_D = QString("%1").arg(path_ID);
                qtts01.append(I_D);
                qtts01.append(",");
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
                qtts01.append(I_D);
                qtts01.append(",");
                qtts0tw.append(I_D);
                qtts0tw.append(",");
            }
            }
            QString I_D;
            path_ID++;
            I_D = QString("%1").arg(path_ID);
            qtts01.append(I_D);
            qtts01.append(")");
            qtts0tw.append(I_D);
            qtts0tw.append(")");
            //qDebug() << qtts01;

            o = db.exec(qtts01);
            if(rdb.isOpen())
            {
            r3 = rdb.exec(qtts0tw);
            }

       // qDebug() <<"LEFT= "<< LEFT;
        if(LEFT <= 11) //Less than 10 points between source and destination
        {
            int LENL = 0;
            //qDebug() <<"LEN= "<< LEN <<"PATH.length()= "<<PATH.length();
            QString qtts02 = "INSERT INTO pathInfoTable (pathID,nextID1,nextID2,nextID3,nextID4,nextID5,nextID6,nextID7,nextID8,nextID9,nextID10,nextpathID) VALUES (";
            QString qtts0te = "INSERT INTO scheduled_routes (pathid,next2,next3,next4,next5,next6,next7,next8,next9,next10,next11,nextpath) VALUES (";
            if(LENL == 0)
            {
                QString I_D;
                I_D = QString("%1").arg(path_ID);
                qtts02.append(I_D);
                qtts02.append(",");
                qtts0te.append(I_D);
                qtts0te.append(",");
            }
            for (LEN = LEN;LEN != PATH.length();LEN++ )
            {
                    QString I_D;
                    I_D = QString("'%1'").arg(PATH.value(LEN));
                    qtts02.append(I_D);
                    qtts02.append(",");
                    qtts0te.append(I_D);
                    qtts0te.append(",");
                    LENL++;
            }

            //qDebug() <<"LEN= "<< LEN;
                for (int C_ID = LEN;C_ID != tot2; C_ID++)
                {
                    //qDebug() <<"C_ID= "<< C_ID;
                    qtts01.append("NULL");
                    qtts01.append(",");
                    qtts0te.append("NULL");
                   // qDebug() <<"C_ID= "<< C_ID;
                    qtts02.append("'NULL'");
                    qtts02.append(",");
                    qtts0te.append("'NULL'");
                    qtts0te.append(",");
                }
            QString I_D;
            I_D = QString("%1").arg(path_ID);
            qtts02.append(I_D);
            qtts02.append(")");
            qtts0te.append(I_D);
            qtts0te.append(")");
            //qDebug() << qtts02;

            o = db.exec(qtts02);
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

/***************************************************************************************************************************/

void MainWindow::DEL_ALL_OLD_PATH()
{
    int sts1/*,sts2*/,sts3,sts4/*,sts5*/;

    QString string_s1;

    QString tqtps1 = QString("SELECT pathID FROM Trains");
    TRAIN = db.exec(tqtps1);

    int Test = sts1+1;

    for(;TRAIN.next() == 1;) //If it is 1 it contains data
    {
    sts1 = TRAIN.value(0).toInt();

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

    if(string_s1=="NULL")
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
    QString CT_Train = ui->trainBox->currentText();
    if (CT_Train == "")
    {
        path_ID = 1;
    }
/*
    QString stqs1,stqs2;
    int sts1,sts2;
    QString tqtps1 = QString("SELECT ID,next,pathID FROM Trains");
    QString smi3 = QString("SELECT pathID,nextID1 from %1").arg("pathInfoTable");
    Path2 = db.exec(smi3);
    TRAIN = db.exec(tqtps1);

    //Check each train
    for(;TRAIN.next() == 1;) //If it is 1 it contains data
    {
        sts2 = TRAIN.value(2).toInt();
        //stqs1 = TRAIN.value(0).toString();
        for(;Path2.next() == 1;)
        {
            sts1 = Path2.value(0).toInt();
            stqs1 = Path2.value(1).toString();
            if(sts1 == sts2)
            {
                stqs2 = stqs1;
            }
        }
        QString train_up2 = QString("UPDATE Trains SET next='%1' WHERE pathID=%2").arg(stqs2).arg(sts2);
        //TRAIN = db.exec(train_up2);
    }*/

    tmodel3 = new QSqlTableModel( this, db );
    ui->trainView->setModel(tmodel3);
    tmodel3->setTable("Trains");
    tmodel3->select();

    tmodel = new QSqlTableModel( this, db );
    ui->tableView->setModel(tmodel);
    tmodel->setTable("pathInfoTable");
    tmodel->select();

    if(rdb.isOpen())
    {
    overrideCheck = rdb.exec("SELECT * FROM override_status WHERE mode;"); //check team 5 for override, if overriden, do nothing
    }

    bool stateCheck = false;


    while(overrideCheck.next() == 1)
    {
        if (overrideCheck.value(0).isValid())
            stateCheck = overrideCheck.value(0).toBool();
    }

    if(!stateCheck) //not overridden, operate normally
    {
        if(overrideStatus)
        {
            QString DEL_1,DEL_2,DEL_3;

            //If returning from override, state of track must be re-established.
            QMessageBox clearWarning;
            clearWarning.setText("Notice: Returning from override. The state of the track is now unknown and will be cleared.");
            clearWarning.exec();

            //These variables have to be initialized in sql.cpp
            //l1-l5 are used for db.exec
            //r1-r5 are used for rdb.exec

            //Remote Databases
            if(rdb.isOpen())
            {
            //Clear State from databases
            DEL_1 = QString("DELETE FROM schedule_train_info");
            DEL_2 = QString("DELETE FROM scheduled_routes");
            DEL_3 = QString("DELETE FROM scheduled_train_info");
            r1=rdb.exec(DEL_1);
            r2=rdb.exec(DEL_2);
            r3=rdb.exec(DEL_3);
            }

            //Local Databases
            DEL_1 = QString("DELETE FROM Trains");
            DEL_2 = QString("DELETE FROM trainInfoTable");
            DEL_3 = QString("DELETE FROM pathInfoTable");
            l1=db.exec(DEL_1);
            l2=db.exec(DEL_2);
            l3=db.exec(DEL_3);

            //Clear State from program variables
            PATH.clear();
            ui->trainBox->clear();

            QPalette* overridePalette = new QPalette();
            overridePalette->setColor(QPalette::WindowText,Qt::blue);
            ui->overwrite_statusLabel->setPalette(*overridePalette);
            ui->overwrite_statusLabel->setText("Normal");

            overrideStatus = false;
        }
        else
        {//----------------------------------------------------------THIS IS IT BOYS WHERE WE MAKE THE TRAINS MOVE----------------------------------------------------------------------------------------------------------------
            QString currentID, currentStart,currentDirection,currentDestination,currentNext,currentPath;
            QString nextNext1, nextNext2;//, nextNext3;

            QString BLAH1, BLAH3;//, BLAH3;
            QString BLAH = QString("SELECT ID FROM Trains");
            runSchedQuery = db.exec(BLAH);

            //Will enter this when there is something selected in runSchedQuery
            while(runSchedQuery.next()==1)
            {
                currentID = runSchedQuery.value(0).toString();

                BLAH1 = QString("SELECT START, Direction, Destination, next, PathID FROM Trains WHERE ID='%1'").arg(currentID);
                runSchedQuery1 = db.exec(BLAH1);

                runSchedQuery1.next();

                currentStart = runSchedQuery1.value(0).toString();
                currentDirection = runSchedQuery1.value(1).toString();

                if(runSchedQuery1.value(1).isNull())
                        currentDirection = "NULL";
                else
                    currentDirection = runSchedQuery1.value(1).toString();

                if(runSchedQuery1.value(2).isNull())
                        currentDestination = "NULL";
                else
                    currentDestination = runSchedQuery1.value(2).toString();

                if(runSchedQuery1.value(3).isNull())
                    currentNext = "NULL";
                else
                    currentNext = runSchedQuery1.value(3).toString();

                if (runSchedQuery1.value(4).isNull())
                    currentPath = "NULL";
                else
                    currentPath = runSchedQuery1.value(4).toString();


                if(currentStart == currentDestination || ((currentNext == "NULL" || currentNext == "") && (currentPath == "NULL" || currentPath == "999" || currentPath =="0")))//If the train is already at its destination, do nothing
                {
                    break;
                    qDebug() << "WAWAWA";
                }

                if(currentPath != "NULL" && currentPath != "999" && currentPath !="0")//if a path exists, check it for details
                {
                    BLAH1 = QString("SELECT nextID1 FROM pathInfoTable WHERE pathID='%1'").arg(currentPath);
                    runSchedQuery1 = db.exec(BLAH1);//fix path table order problem

                    runSchedQuery1.next();
                    if(runSchedQuery1.value(0).isValid())
                        nextNext1 = runSchedQuery1.value(0).toString();

                    if(currentNext==nextNext1 || currentNext=="NULL"|| currentNext=="EMPTY")//If they are the same, some shifting needs to happen
                    {
                        l1 = db.exec("UPDATE Trains SET next='"+nextNext1+"' WHERE ID='"+currentID+"';");
                        if(rdb.isOpen())
                        {
                            r2 = rdb.exec("UPDATE scheduled_train_info SET next='"+nextNext1+"' WHERE id='"+currentID+"';");
                        }

                        QString thisPath, nextPath;
                        thisPath = currentPath;
                        l1 = db.exec("UPDATE pathInfoTable SET nextID1='NULL' WHERE pathID='"+thisPath+"';");
                        if(rdb.isOpen())
                        {
                            r1 = rdb.exec("UPDATE scheduled_routes SET next2='NULL' WHERE pathID='"+thisPath+"';");
                        }


                        /*if(currentDirection==nextNext1)
                        {
                            currentDirection=nextNext2;
                            currentStart=currentNext;

                            l2 = db.exec("UPDATE `Trains` SET `START`='"+currentStart+"', `Direction`='"+currentDirection+"', `next`=NULL WHERE `ID`='"+currentID+"';");
                            if(rdb.isOpen())
                            {
                                r2 = rdb.exec("UPDATE `Trains` SET `current`='"+currentStart+"', `next`=NULL WHERE `id`='"+currentID+"';");
                            }
                        }
                        else
                        {
                            currentDirection=currentStart;
                            currentStart=currentDestination;
                            currentNext="NULL";

                            l2 = db.exec("UPDATE `Trains` SET `START`='"+currentStart+"', `Direction`='"+currentDirection+"', `next`=NULL WHERE `ID`='"+currentID+"';");
                            if(rdb.isOpen())
                            {
                                r2 = rdb.exec("UPDATE `Trains` SET `current`='"+currentStart+"', `next`=NULL WHERE `id`='"+currentID+"';");
                            }
                        }*/

                        while(1)//the only way out is to earn a break, also the idea here is Matroska style up until entry 10
                        {
                            l1 = db.exec("SELECT nextID2 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                            l1.next();
                            if (l1.value(0).isNull())//this row is done. Delete it, change previous row, and if no previous row, remove path from Train table
                            {
                                if (nextPath==thisPath) //no previous path, remove row from train table
                                {
                                    l1 = db.exec("DELETE FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                    if(rdb.isOpen())
                                    {
                                        r1 = rdb.exec("DELETE FROM scheduled_routes WHERE pathID='"+thisPath+"';");
                                    }

                                    l1 = db.exec("UPDATE Trains SET pathID='999' WHERE ID='"+currentID+"';");
                                    if(rdb.isOpen())
                                    {
                                        r1 = rdb.exec("UPDATE scheduled_train_info SET pathID='999' WHERE pathID='"+currentID+"';");
                                    }
                                }
                                else                    //there is a previous row, delete current one, and tie up previous row.
                                {
                                    l1 = db.exec("DELETE FROM pathInfoTable WHERE pathID='"+nextPath+"';");
                                    if(rdb.isOpen())
                                    {
                                        r1 = rdb.exec("DELETE FROM scheduled_routes WHERE pathID='"+nextPath+"';");
                                    }

                                    l1 = db.exec("UPDATE pathInfoTable SET nextpathID='"+thisPath+"' WHERE pathID='"+thisPath+"';");
                                    if(rdb.isOpen())
                                    {
                                        r1 = rdb.exec("UPDATE scheduled_routes SET nextpath='"+thisPath+"' WHERE pathID='"+thisPath+"';");
                                    }
                                }
                                break;
                            }
                            else
                            {
                                nextPath = thisPath;//since this path isn't empty, no need to backtrack

                                nextNext2 = l1.value(0).toString();


                                l1 = db.exec("UPDATE pathInfoTable SET nextID1='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                if(rdb.isOpen())
                                {
                                    r1 = rdb.exec("UPDATE scheduled_routes SET next2='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                }

                                l1 = db.exec("UPDATE pathInfoTable SET nextID2='NULL' WHERE pathID='"+thisPath+"';");
                                if(rdb.isOpen())
                                {
                                    r1 = rdb.exec("UPDATE scheduled_routes SET next3='NULL' WHERE pathID='"+thisPath+"';");
                                }

                                l1 = db.exec("SELECT nextID3 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                l1.next();
                                if(l1.value(0).isNull())
                                    break;
                                else
                                {
                                    nextNext2 = l1.value(0).toString();
                                    l1 = db.exec("UPDATE pathInfoTable SET nextID2='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                    if(rdb.isOpen())
                                    {
                                        r1 = rdb.exec("UPDATE scheduled_routes SET next3='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                    }

                                    l1 = db.exec("UPDATE pathInfoTable SET nextID3 = 'NULL' WHERE pathID='"+thisPath+"';");
                                    if(rdb.isOpen())
                                    {
                                        r1 = rdb.exec("UPDATE scheduled_routes SET next4='NULL' WHERE pathID='"+thisPath+"';");
                                    }

                                    l1 = db.exec("SELECT nextID4 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                    l1.next();
                                    if(l1.value(0).isNull())
                                        break;
                                    else
                                    {
                                        nextNext2 = l1.value(0).toString();
                                        l1 = db.exec("UPDATE pathInfoTable SET nextID3='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                        if(rdb.isOpen())
                                        {
                                            r1 = rdb.exec("UPDATE scheduled_routes SET next4='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                        }

                                        l1 = db.exec("UPDATE pathInfoTable SET nextID4='NULL' WHERE pathID='"+thisPath+"';");
                                        if(rdb.isOpen())
                                        {
                                            r1 = rdb.exec("UPDATE scheduled_routes SET next5='NULL' WHERE pathID='"+thisPath+"';");
                                        }

                                        l1 = db.exec("SELECT nextID5 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                        l1.next();
                                        if(l1.value(0).isNull())
                                            break;
                                        else
                                        {
                                            nextNext2 = l1.value(0).toString();
                                            l1 = db.exec("UPDATE pathInfoTable SET nextID4='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                            if(rdb.isOpen())
                                            {
                                                r1 = rdb.exec("UPDATE scheduled_routes SET next5='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                            }

                                            l1 = db.exec("UPDATE pathInfoTable SET nextID5='NULL' WHERE pathID='"+thisPath+"';");
                                            if(rdb.isOpen())
                                            {
                                                r1 = rdb.exec("UPDATE scheduled_routes SET next6='NULL' WHERE pathID='"+thisPath+"';");
                                            }

                                            l1 = db.exec("SELECT nextID6 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                            l1.next();
                                            if(l1.value(0).isNull())
                                                break;
                                            else
                                            {
                                                nextNext2 = l1.value(0).toString();
                                                l1 = db.exec("UPDATE pathInfoTable SET nextID5='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                if(rdb.isOpen())
                                                {
                                                    r1 = rdb.exec("UPDATE scheduled_routes SET next6='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                }

                                                l1 = db.exec("UPDATE pathInfoTable SET nextID6='NULL' WHERE pathID='"+thisPath+"';");
                                                if(rdb.isOpen())
                                                {
                                                    r1 = rdb.exec("UPDATE scheduled_routes SET next7='NULL' WHERE pathID='"+thisPath+"';");
                                                }

                                                l1 = db.exec("SELECT nextID7 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                                l1.next();
                                                if(l1.value(0).isNull())
                                                    break;
                                                else
                                                {
                                                    nextNext2 = l1.value(0).toString();
                                                    l1 = db.exec("UPDATE pathInfoTable SET nextID6='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                    if(rdb.isOpen())
                                                    {
                                                        r1 = rdb.exec("UPDATE scheduled_routes SET next7='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                    }

                                                    l1 = db.exec("UPDATE pathInfoTable SET nextID7='NULL' WHERE pathID='"+thisPath+"';");
                                                    if(rdb.isOpen())
                                                    {
                                                        r1 = rdb.exec("UPDATE scheduled_routes SET next8='NULL' WHERE pathID='"+thisPath+"';");
                                                    }

                                                    l1 = db.exec("SELECT nextID8 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                                    l1.next();
                                                    if(l1.value(0).isNull())
                                                        break;
                                                    else
                                                    {
                                                        nextNext2 = l1.value(0).toString();
                                                        l1 = db.exec("UPDATE pathInfoTable SET nextID7='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                        if(rdb.isOpen())
                                                        {
                                                            r1 = rdb.exec("UPDATE scheduled_routes SET next8='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                        }

                                                        l1 = db.exec("UPDATE pathInfoTable SET nextID8='NULL' WHERE pathID='"+thisPath+"';");
                                                        if(rdb.isOpen())
                                                        {
                                                            r1 = rdb.exec("UPDATE scheduled_routes SET next9='NULL' WHERE pathID='"+thisPath+"';");
                                                        }

                                                        l1 = db.exec("SELECT nextID9 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                                        l1.next();
                                                        if(l1.value(0).isNull())
                                                            break;
                                                        else
                                                        {
                                                            nextNext2 = l1.value(0).toString();
                                                            l1 = db.exec("UPDATE pathInfoTable SET nextID8='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                            if(rdb.isOpen())
                                                            {
                                                                r1 = rdb.exec("UPDATE scheduled_routes SET next9='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                            }

                                                            l1 = db.exec("UPDATE pathInfoTable SET nextID9='NULL' WHERE pathID='"+thisPath+"';");
                                                            if(rdb.isOpen())
                                                            {
                                                                r1 = rdb.exec("UPDATE scheduled_routes SET next10='NULL' WHERE pathID='"+thisPath+"';");
                                                            }

                                                            l1 = db.exec("SELECT nextID10 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                                            l1.next();
                                                            if(l1.value(0).isNull())
                                                                break;
                                                            else
                                                            {
                                                                nextNext2 = l1.value(0).toString();
                                                                l1 = db.exec("UPDATE pathInfoTable SET nextID9='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                                if(rdb.isOpen())
                                                                {
                                                                    r1 = rdb.exec("UPDATE scheduled_routes SET next10='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                                };

                                                                l1 = db.exec("UPDATE pathInfoTable SET nextID10='NULL' WHERE pathID='"+thisPath+"';");
                                                                if(rdb.isOpen())
                                                                {
                                                                    r1 = rdb.exec("UPDATE scheduled_routes SET next11='NULL' WHERE pathID='"+thisPath+"';");
                                                                };

                                                                l1 = db.exec("SELECT nextpathID FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                                                l1.next();
                                                                nextPath = l1.value(0).toString();
                                                                if(thisPath == nextPath)
                                                                {
                                                                    break;
                                                                }
                                                                else
                                                                {
                                                                    l1 = db.exec("SELECT nextID1 FROM pathInfoTable WHERE pathID='"+nextPath+"';");
                                                                    l1.next();
                                                                    if(l1.value(0).isValid())
                                                                    {
                                                                        if(l1.value(0).isNull())
                                                                        {
                                                                            l1 = db.exec("DELETE FROM pathInfoTable WHERE pathID='"+nextPath+"';");
                                                                            if(rdb.isOpen())
                                                                            {
                                                                                r1 = rdb.exec("DELETE FROM scheduled_routes WHERE pathID='"+nextPath+"';");
                                                                            }

                                                                            l1 = db.exec("UPDATE pathInfoTable SET nextpathID='"+thisPath+"' WHERE pathID='"+thisPath+"';");
                                                                            if(rdb.isOpen())
                                                                            {
                                                                                r1 = rdb.exec("UPDATE scheduled_routes SET nextpath='"+thisPath+"' WHERE pathID='"+thisPath+"';");
                                                                            }
                                                                        }
                                                                        else
                                                                        {
                                                                            nextNext1 = l1.value(0).toString();

                                                                            l1 = db.exec("UPDATE pathInfoTable SET nextID1='"+nextNext1+"' WHERE pathID='"+thisPath+"';");
                                                                            if(rdb.isOpen())
                                                                            {
                                                                                r1 = rdb.exec("UPDATE scheduled_routes SET next2='"+nextNext1+"' WHERE pathID='"+thisPath+"';");
                                                                            }


                                                                            l1 = db.exec("UPDATE pathInfoTable SET nextID1='NULL' WHERE pathID='"+nextPath+"';");
                                                                            if(rdb.isOpen())
                                                                            {
                                                                                r1 = rdb.exec("UPDATE scheduled_routes SET next2='NULL' WHERE pathID='"+nextPath+"';");
                                                                            }

                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        bool move_on;
                        if(rdb.isOpen())
                        {
                            r3 = rdb.exec("SELECT status FROM track_ds WHERE id='"+currentNext+"';");
                            r3.next();
                            move_on = (r3.value(0).toString()=="1");
                        }
                        else
                        {
                            move_on=false;
                        }

                        if(move_on || Testing_path == 1)//If the next piece is occupied, move forward on path
                        {
                            l1 = db.exec("UPDATE Trains SET next='"+nextNext1+"' WHERE ID='"+currentID+"';");
                            if(rdb.isOpen())
                            {
                                r2 = rdb.exec("UPDATE scheduled_train_info SET next='"+nextNext1+"' WHERE `id`='"+currentID+"';");
                            }

                            QString thisPath, nextPath;
                            thisPath = currentPath;
                            l1 = db.exec("UPDATE pathInfoTable SET nextID1='NULL' WHERE pathID='"+thisPath+"';");
                            if(rdb.isOpen())
                            {
                                r1 = rdb.exec("UPDATE scheduled_routes SET next2='NULL' WHERE pathID='"+thisPath+"';");
                            }


                            if(currentDirection==currentNext)//facing forward
                            {
                                currentDirection=nextNext1;
                                currentStart=currentNext;

                                l2 = db.exec("UPDATE Trains SET START='"+currentStart+"', Direction='"+currentDirection+"' WHERE ID='"+currentID+"';");
                                if(rdb.isOpen())
                                {
                                    r2 = rdb.exec("UPDATE Trains SET current='"+currentStart+"' WHERE id='"+currentID+"';");
                                }
                            }
                            else//facing backward
                            {
                                currentDirection=currentStart;
                                currentStart=currentNext;

                                l2 = db.exec("UPDATE Trains SET START='"+currentStart+"', Direction='"+currentDirection+"', next='NULL' WHERE `ID`='"+currentID+"';");
                                if(rdb.isOpen())
                                {
                                    r2 = rdb.exec("UPDATE Trains SET current='"+currentStart+"', next='NULL' WHERE id='"+currentID+"';");
                                }
                            }

                            while(1)//the only way out is to earn a break, also the idea here is Matroska style up until entry 10
                            {
                                l1 = db.exec("SELECT nextID2 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                l1.next();
                                if (l1.value(0).isNull())//this row is done. Delete it, change previous row, and if no previous row, remove path from Train table
                                {
                                    if (nextPath==thisPath) //no previous path, remove row from train table
                                    {
                                        l1 = db.exec("DELETE FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                        if(rdb.isOpen())
                                        {
                                            r1 = rdb.exec("DELETE FROM scheduled_routes WHERE pathID='"+thisPath+"';");
                                        }

                                        l1 = db.exec("UPDATE Trains SET pathID='999' WHERE ID='"+currentID+"';");
                                        if(rdb.isOpen())
                                        {
                                            r1 = rdb.exec("UPDATE scheduled_train_info SET pathID='999' WHERE pathID='"+currentID+"';");
                                        }
                                    }
                                    else                    //there is a previous row, delete current one, and tie up previous row.
                                    {
                                        l1 = db.exec("DELETE FROM pathInfoTable WHERE pathID='"+nextPath+"';");
                                        if(rdb.isOpen())
                                        {
                                            r1 = rdb.exec("DELETE FROM scheduled_routes WHERE pathID='"+nextPath+"';");
                                        }

                                        l1 = db.exec("UPDATE pathInfoTable SET nextpathID='"+thisPath+"' WHERE pathID='"+thisPath+"';");
                                        if(rdb.isOpen())
                                        {
                                            r1 = rdb.exec("UPDATE scheduled_routes SET nextpath='"+thisPath+"' WHERE pathID='"+thisPath+"';");
                                        }
                                    }
                                    break;
                                }
                                else
                                {
                                    nextPath = thisPath;//since this path isn't empty, no need to backtrack

                                    nextNext2 = l1.value(0).toString();


                                    l1 = db.exec("UPDATE pathInfoTable SET nextID1='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                    if(rdb.isOpen())
                                    {
                                        r1 = rdb.exec("UPDATE scheduled_routes SET next2='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                    }

                                    l1 = db.exec("UPDATE pathInfoTable SET nextID2='NULL' WHERE pathID='"+thisPath+"';");
                                    if(rdb.isOpen())
                                    {
                                        r1 = rdb.exec("UPDATE scheduled_routes SET next3='NULL' WHERE pathID='"+thisPath+"';");
                                    }

                                    l1 = db.exec("SELECT nextID3 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                    l1.next();
                                    if(l1.value(0).isNull())
                                        break;
                                    else
                                    {
                                        nextNext2 = l1.value(0).toString();
                                        l1 = db.exec("UPDATE pathInfoTable SET nextID2='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                        if(rdb.isOpen())
                                        {
                                            r1 = rdb.exec("UPDATE scheduled_routes SET next3='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                        }

                                        l1 = db.exec("UPDATE pathInfoTable SET nextID3='NULL' WHERE pathID='"+thisPath+"';");
                                        if(rdb.isOpen())
                                        {
                                            r1 = rdb.exec("UPDATE scheduled_routes SET next4='NULL' WHERE pathID='"+thisPath+"';");
                                        }

                                        l1 = db.exec("SELECT nextID4 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                        l1.next();
                                        if(l1.value(0).isNull())
                                            break;
                                        else
                                        {
                                            nextNext2 = l1.value(0).toString();
                                            l1 = db.exec("UPDATE pathInfoTable SET nextID3='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                            if(rdb.isOpen())
                                            {
                                                r1 = rdb.exec("UPDATE scheduled_routes SET next4='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                            }

                                            l1 = db.exec("UPDATE pathInfoTable SET nextID4='NULL' WHERE pathID='"+thisPath+"';");
                                            if(rdb.isOpen())
                                            {
                                                r1 = rdb.exec("UPDATE scheduled_routes SET next5='NULL' WHERE pathID='"+thisPath+"';");
                                            }

                                            l1 = db.exec("SELECT nextID5 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                            l1.next();
                                            if(l1.value(0).isNull())
                                                break;
                                            else
                                            {
                                                nextNext2 = l1.value(0).toString();
                                                l1 = db.exec("UPDATE pathInfoTable SET nextID4='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                if(rdb.isOpen())
                                                {
                                                    r1 = rdb.exec("UPDATE scheduled_routes SET next5='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                }

                                                l1 = db.exec("UPDATE pathInfoTable SET nextID5='NULL' WHERE pathID='"+thisPath+"';");
                                                if(rdb.isOpen())
                                                {
                                                    r1 = rdb.exec("UPDATE scheduled_routes SET next6='NULL' WHERE pathID='"+thisPath+"';");
                                                }

                                                l1 = db.exec("SELECT nextID6 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                                l1.next();
                                                if(l1.value(0).isNull())
                                                    break;
                                                else
                                                {
                                                    nextNext2 = l1.value(0).toString();
                                                    l1 = db.exec("UPDATE pathInfoTable SET nextID5='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                    if(rdb.isOpen())
                                                    {
                                                        r1 = rdb.exec("UPDATE scheduled_routes SET next6='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                    }

                                                    l1 = db.exec("UPDATE pathInfoTable SET nextID6='NULL' WHERE pathID='"+thisPath+"';");
                                                    if(rdb.isOpen())
                                                    {
                                                        r1 = rdb.exec("UPDATE scheduled_routes SET next7='NULL' WHERE pathID='"+thisPath+"';");
                                                    }

                                                    l1 = db.exec("SELECT nextID7 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                                    l1.next();
                                                    if(l1.value(0).isNull())
                                                        break;
                                                    else
                                                    {
                                                        nextNext2 = l1.value(0).toString();
                                                        l1 = db.exec("UPDATE pathInfoTable SET nextID6='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                        if(rdb.isOpen())
                                                        {
                                                            r1 = rdb.exec("UPDATE scheduled_routes SET next7='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                        }

                                                        l1 = db.exec("UPDATE pathInfoTable SET nextID7='NULL' WHERE pathID='"+thisPath+"';");
                                                        if(rdb.isOpen())
                                                        {
                                                            r1 = rdb.exec("UPDATE scheduled_routes SET next8='NULL' WHERE pathID='"+thisPath+"';");
                                                        }

                                                        l1 = db.exec("SELECT nextID8 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                                        l1.next();
                                                        if(l1.value(0).isNull())
                                                            break;
                                                        else
                                                        {
                                                            nextNext2 = l1.value(0).toString();
                                                            l1 = db.exec("UPDATE pathInfoTable SET nextID7='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                            if(rdb.isOpen())
                                                            {
                                                                r1 = rdb.exec("UPDATE scheduled_routes SET next8='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                            }

                                                            l1 = db.exec("UPDATE pathInfoTable SET nextID8='NULL' WHERE pathID='"+thisPath+"';");
                                                            if(rdb.isOpen())
                                                            {
                                                                r1 = rdb.exec("UPDATE scheduled_routes SET next9='NULL' WHERE pathID='"+thisPath+"';");
                                                            }

                                                            l1 = db.exec("SELECT nextID9 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                                            l1.next();
                                                            if(l1.value(0).isNull())
                                                                break;
                                                            else
                                                            {
                                                                nextNext2 = l1.value(0).toString();
                                                                l1 = db.exec("UPDATE pathInfoTable SET nextID8='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                                if(rdb.isOpen())
                                                                {
                                                                    r1 = rdb.exec("UPDATE scheduled_routes SET next9='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                                }

                                                                l1 = db.exec("UPDATE pathInfoTable SET nextID9='NULL' WHERE pathID='"+thisPath+"';");
                                                                if(rdb.isOpen())
                                                                {
                                                                    r1 = rdb.exec("UPDATE scheduled_routes SET next10='NULL' WHERE pathID='"+thisPath+"';");
                                                                }

                                                                l1 = db.exec("SELECT nextID10 FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                                                l1.next();
                                                                if(l1.value(0).isNull())
                                                                {
                                                                    break;
                                                                }
                                                                else
                                                                {
                                                                    nextNext2 = l1.value(0).toString();
                                                                    l1 = db.exec("UPDATE pathInfoTable SET nextID9='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                                    if(rdb.isOpen())
                                                                    {
                                                                        r1 = rdb.exec("UPDATE scheduled_routes SET next10='"+nextNext2+"' WHERE pathID='"+thisPath+"';");
                                                                    };

                                                                    l1 = db.exec("UPDATE pathInfoTable SET nextID10='NULL' WHERE pathID='"+thisPath+"';");
                                                                    if(rdb.isOpen())
                                                                    {
                                                                        r1 = rdb.exec("UPDATE scheduled_routes SET next11='NULL' WHERE pathID='"+thisPath+"';");
                                                                    };
                                                                    l1 = db.exec("SELECT nextpathID FROM pathInfoTable WHERE pathID='"+thisPath+"';");
                                                                    l1.next();
                                                                    nextPath = l1.value(0).toString();
                                                                    qDebug() << "thisPath" <<thisPath;
                                                                    qDebug() << "nextPath" <<nextPath;
                                                                    if(thisPath == nextPath)
                                                                    {
                                                                        break;
                                                                    }
                                                                    else
                                                                    {
                                                                        l1 = db.exec("SELECT nextID1 FROM pathInfoTable WHERE pathID='"+nextPath+"';");
                                                                        l1.next();
                                                                        if(l1.value(0).isValid())
                                                                        {
                                                                            if(l1.value(0).isNull())
                                                                            {
                                                                                l1 = db.exec("DELETE FROM pathInfoTable WHERE pathID='"+nextPath+"';");
                                                                                if(rdb.isOpen())
                                                                                {
                                                                                    r1 = rdb.exec("DELETE FROM scheduled_routes WHERE pathID='"+nextPath+"';");
                                                                                };

                                                                                l1 = db.exec("UPDATE pathInfoTable SET nextpathID='"+thisPath+"' WHERE pathID='"+thisPath+"';");
                                                                                if(rdb.isOpen())
                                                                                {
                                                                                    r1 = rdb.exec("UPDATE scheduled_routes SET nextpath='"+thisPath+"' WHERE pathID='"+thisPath+"';");
                                                                                }
                                                                            }
                                                                            else
                                                                            {
                                                                                nextNext1 = l1.value(0).toString();

                                                                                l1 = db.exec("UPDATE pathInfoTable SET nextID1='"+nextNext1+"' WHERE pathID='"+thisPath+"';");
                                                                                if(rdb.isOpen())
                                                                                {
                                                                                    r1 = rdb.exec("UPDATE scheduled_routes SET next2='"+nextNext1+"' WHERE pathID='"+thisPath+"';");
                                                                                }


                                                                                l1 = db.exec("UPDATE pathInfoTable SET nextID1='NULL' WHERE pathID='"+nextPath+"';");
                                                                                if(rdb.isOpen())
                                                                                {
                                                                                    r1 = rdb.exec("UPDATE scheduled_routes SET next2='NULL' WHERE pathID='"+nextPath+"';");
                                                                                }

                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        //move_on;
                        if(rdb.isOpen())
                        {
                            r3 = rdb.exec("SELECT status FROM track_ds WHERE id='"+currentStart+"';");
                            r3.next();
                            move_on = (r3.value(0).toString()=="1");
                        }
                        else
                            move_on=false;

                        if(move_on)
                        {
                            if(currentDirection==currentNext)
                            {
                                if(rdb.isOpen())
                                {
                                BLAH3 = QString("INSERT INTO req_macro (`macro`, `arg1`, `arg2`)\nVALUES ('TRAIN_REQ', '%1', '50';").arg(currentID);
                                runSchedQuery3 = rdb.exec(BLAH3);//throttle zero
                                if(!runSchedQuery.isValid())
                                    qDebug() << "Error: Throttle write failed\n";
                                }
                            }
                            else
                            {
                                if(rdb.isOpen())
                                {
                                BLAH3 = QString("INSERT INTO req_macro (`macro`, `arg1`, `arg2`)\nVALUES ('TRAIN_REQ', '%1', '-50';").arg(currentID);
                                runSchedQuery3 = rdb.exec(BLAH3);//throttle zero
                                if(!runSchedQuery.isValid())
                                     qDebug() << "Error: Throttle write failed\n";
                                }
                            }
                        }

                    }
                }
                else
                {//The only situation where a path doesn't exist, and the break isn't reached, is when the next space IS destination
                    if(currentDestination == currentNext) // if next piece is the destination
                    {
                        //check if destination detection section is occupied, if so, throttle 0, clear next, and set start to destination. DONE BABY.
                        BLAH1 = QString("SELECT status FROM track_ds WHERE id='%1'").arg(currentDestination);
                        r1 = db.exec(BLAH1);

                        while(r1.next()==1)
                        {
                            if(r1.value(0).isValid())
                            {
                                QString arrival = r1.value(0).toString();

                                if(arrival == "1")
                                {//arrived at destination, stop train, and set tables to know the destination was reached.
                                    if(rdb.isOpen())
                                    {
                                    BLAH3 = QString("INSERT INTO req_macro (`macro`, `arg1`, `arg2`)\nVALUES ('SLOT_REQ', '%1', '0';").arg(currentID);
                                    runSchedQuery3 = rdb.exec(BLAH3);//throttle zero
                                    }

                                    if(currentDirection==currentDestination)
                                    {
                                        //Set Start to old Destination
                                        //No new path -- set next piece to Null
                                        currentStart=currentDestination;
                                        currentNext="NULL";
                                                                                                     //Next Piece //Prev Piece
                                        QString Sts1 = QString("SELECT Current, NumberOfConnections, Connection1, Connection2, Connection3 from %1 WHERE Current='%2'").arg("DS_Connectivity").arg(currentDestination);
                                        o = db.exec(Sts1);
                                        o.next();
                                        //Next
                                        QString sts1 = o.value(2).toString();
                                        //Backward
                                        QString sts2 = o.value(3).toString();
                                        qDebug() << sts1 << sts2;

                                        //Set currentDirection
                                        currentDirection = sts1; //Next Piece

                                        l2 = db.exec("UPDATE Train SET START='"+currentStart+"',Direction='"+currentDirection+"', next='NULL' WHERE ID='"+currentID+"';");
                                        if(rdb.isOpen())
                                        {
                                            r2 = rdb.exec("UPDATE Trains SET current='"+currentStart+"', next='NULL' WHERE id='"+currentID+"';");
                                        }
                                    }
                                    else
                                    {
                                        currentDirection=currentStart;
                                        currentStart=currentDestination;
                                        currentNext="NULL";

                                        l2 = db.exec("UPDATE Trains SET START='"+currentStart+"', Direction='"+currentDirection+"', next='NULL' WHERE ID='"+currentID+"';");
                                        if(rdb.isOpen())
                                        {
                                            r2 = rdb.exec("UPDATE Trains SET current='"+currentStart+"', next='NULL' WHERE id='"+currentID+"';");
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if(rdb.isOpen())
                                {
                                BLAH3 = QString("INSERT INTO req_macro (`macro`, `arg1`, `arg2`)\nVALUES ('TRAIN_REQ', '%1', '0';").arg(currentID);
                                runSchedQuery3 = rdb.exec(BLAH3);//throttle zero

                                BLAH3 = QString("INSERT INTO req_macro (`macro`, `arg1`, `arg2`)\nVALUES ('TRAIN_REQ', '%1', '0';").arg(currentID);
                                runSchedQuery3 = rdb.exec(BLAH3);//throttle zero
                                }
                                qDebug() << "Error: Cannot read DS " << currentDestination << ". Stopping train." ;
                            }
                        }
                        ;//if destination is not occupied, throttle to 10. TOO CLOSE FOR CONFORT YO...
                    }
                    else
                    {//This section of code should not be possible to reach, so.... stop the train?
                        if(rdb.isOpen())
                        {
                        BLAH3 = QString("INSERT INTO req_macro (`macro`, `arg1`, `arg2`)\nVALUES ('SLOT_REQ', '%1', '0';").arg(currentID);
                        runSchedQuery3 = rdb.exec(BLAH3);//throttle zero
                        }
                        qDebug() << "Error: Something has gone horribly awry...";
                    }
                }


                //req_switch controls switches, col1 = ID (is the # for switch), col2 = position (1 or 0 NOTE 1 is through, 0 is bypass);
                //req_macro controls trains, col1 = macro (use TRAIN_REQ), col2 = arg1 (train#), col3 = arg2 (speed% is -100 to +100)
                //track_ds for detection sections, col1 = id (ds style), col2 = status (1 = occupied, 0 = empty), col3 = ds_from (ignore), col4 = ds_to (ignore)
            }
        }
    }
    else
    {
        if(!overrideStatus)  //just now entering override status, change display
        {
            QPalette* overridePalette = new QPalette();
            overridePalette->setColor(QPalette::WindowText,Qt::red);
            ui->overwrite_statusLabel->setPalette(*overridePalette);
            ui->overwrite_statusLabel->setText("Override Active");

            overrideStatus = true;
        }
    } 

//check occupancy data and adjust throttles accordingly,
    //If a schedule cannot be made up with throttle adjustments, trigger reroute
}
