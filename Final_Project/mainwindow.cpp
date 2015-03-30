#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Train Scheduling Application");

    path_ID = 0;

    //mysql --host=pavelow.eng.uah.edu --protocol=tcp --port=33158 --user=root --password=drabroig
/*
    rdb.addDatabase( "QMYSQL", "Remote" );
    rdb.setDatabaseName("Remote");
    rdb.setHostName("pavelow.eng.uah.edu");
    rdb.setPort(33158);
    rdb.setDatabaseName("team4b");
    rdb.setUserName("root");
    rdb.setPassword("drabroig");
*/

    //mysql --host=pavelow.eng.uah.edu --protocol=tcp --port=33155 --user=Team1B_User --password=YaaasGAGA
    //Team1b database and user for testing
    rdb.addDatabase( "QMYSQL", "Remote" );
    rdb.setDatabaseName("Remote");
    rdb.setHostName("pavelow.eng.uah.edu");
    rdb.setPort(33155);
    rdb.setDatabaseName("team1b");
    rdb.setUserName("Team1B_User");
    rdb.setPassword("YaaasGAGA");



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
    connect(ui->scheduleButton,SIGNAL(clicked()),this,SLOT(Update_ScheduleTable()));
    connect(ui->actionView_Trains,SIGNAL(triggered()),this,SLOT(Train_Table()));
    //connect(ui->set_scheduleButton,SIGNAL(clicked()),this,SLOT(Set_Schedule()));
    connect(ui->actionView_DS_Connectivity,SIGNAL(triggered()),this,SLOT(DS_Connectivity_Table()));
    connect(ui->actionView_Tracklisting,SIGNAL(triggered()),this,SLOT(Tracklisting_Table()));
    connect(ui->actionView_Trackinfo,SIGNAL(triggered()),this,SLOT(Trackinfo_Table()));
    connect(ui->actionView_Pathinfo,SIGNAL(triggered()),this,SLOT(Pathinfo_Table()));

    QTimer* persistence = new QTimer();
    QObject::connect(persistence, SIGNAL(timeout()), this, SLOT(check_sched()));
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
    if (ok && !ID.isEmpty())
        ui->trainBox->addItem(ID);

    QString qtts0 = QString("INSERT INTO Trains (ID,Start,Direction,Destination,pathID) VALUES ('%1','%2','%3','%4','%5')").arg(ID).arg(Start).arg(direction).arg("EMPTY").arg(999);
    o = db.exec(qtts0);
    o.next();
    }
}

void MainWindow::Edit_Train()
{
    ID = ui->trainBox->currentText();

    if (ID != "")
    {

    QString VAL1,VAL2,VAL3,VAL4;
    items.clear();
    QString Edit_ID = QString("SELECT ID,Start,Direction,Destination,pathID from %1 WHERE ID='%2'").arg("Trains").arg(ID);
    TRAIN = db.exec(Edit_ID);
    TRAIN.next();
    VAL1 = TRAIN.value(0).toString();
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

    QString tqtps1 = QString("SELECT pathID FROM Trains WHERE ID='%1'").arg(ui->trainBox->currentText());
    TRAIN = db.exec(tqtps1);

    int sts22;

    for(;TRAIN.next() == 1;) //If it is 1 it contains data
    {
    sts22 = TRAIN.value(4).toInt();
    }

    QString ttps1 = QString("DELETE FROM pathInfoTable WHERE pathID=%1").arg(sts22);
    TRAIN2 = db.exec(ttps1);

    QString ttps2 = QString("DELETE FROM pathInfoTable WHERE nextID1='%1'").arg("NULL");
    TRAIN2 = db.exec(ttps2);

    Schedule();
    Update_ScheduleTable();
    DEL_OLD_PATH();

    tmodel = new QSqlTableModel( this, db );
    ui->tableView->setModel(tmodel);
    tmodel->setTable("pathInfoTable");
    tmodel->select();
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
   {

       DEL_OLD_PATH();
       Check_Path_Trains();

       ui->trainBox->removeItem(CI_Train);
       INFO = db.exec(DEL_Train);

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
            //qDebug() << record;
                    for (int field = 0; field < record.count(); ++field)
                    {
                        if(field >= 0 && field !=6 )
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
        QString COL5 = line=in.readLine();
        QString tts0 = QString("INSERT INTO Trains (ID,Start,Direction,Destination,pathID) VALUES ('%1','%2','%3','%4',%5)").arg(COL1).arg(COL2).arg(COL3).arg(COL4).arg(COL5);
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
    DEL_OLD_PATH();
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

void MainWindow::DS_Connectivity_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("DS_Connectivity");
    tmodel->select();
    view->show();
}

void MainWindow::Tracklisting_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("tracklistingTable");
    tmodel->select();
    view->show();
}

void MainWindow::Trackinfo_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("trackInfoTable");
    tmodel->select();
    view->show();
}

void MainWindow::Pathinfo_Table()
{
    tmodel = new QSqlTableModel( this, db );
    view = new QTableView;
    view->setModel(tmodel);
    tmodel->setTable("pathInfoTable");
    tmodel->select();
    view->show();
}


void MainWindow::Update_ScheduleTable()
{
    PATH.begin();
    if(ERROR == 1)
    {

    }
    else
    {
    Check_Path_Trains();
    DEL_OLD_PATH();
    if(PATH.length() <= 11) //Less than 11 points between source and destination
    {
        int LEN;
        QString qtts0 = "INSERT INTO pathInfoTable (pathID,nextID1,nextID2,nextID3,nextID4,nextID5,nextID6,nextID7,nextID8,nextID9,nextID10,nextpathID) VALUES (";
        for (LEN = 0;LEN != PATH.length();LEN++ )
        {
            if(LEN == 0)
            {
                QString I_D;
                I_D = QString("%1").arg(path_ID);
                qtts0.append(I_D);
                qtts0.append(",");
            }
            else
            {
                QString I_D;
                I_D = QString("'%1'").arg(PATH.value(LEN));
                qtts0.append(I_D);
                qtts0.append(",");
            }
        }
        //qDebug() << LEN;
        if(LEN == 11)
        {

        }
        else
        {
            for (int C_ID = LEN;C_ID != 11; C_ID++)
            {
                //qDebug() << C_ID;
                qtts0.append("'NULL'");
                qtts0.append(",");
            }
        }
        QString I_D;
        I_D = QString("%1").arg(path_ID);
        qtts0.append(I_D);
        qtts0.append(")");
        //qDebug() << qtts0;
        INFO = db.exec(qtts0);

        QString train_up = QString("UPDATE Trains SET pathID=%1 WHERE ID='%2'").arg(path_ID).arg(ui->trainBox->currentText());
        TRAIN = db.exec(train_up);
    }
    else //Greater than 10 points between source and destination (Multi-line Path)
    {
        //Must use # of paths for # of full rows of data *each row is 10 nodes

        int LEN = 0;
        int TOT = PATH.length();
        int TOT_LEFT = TOT - LEN;
        if (TOT > 23)
        {
            QString qtts0 = "INSERT INTO pathInfoTable (pathID,nextID1,nextID2,nextID3,nextID4,nextID5,nextID6,nextID7,nextID8,nextID9,nextID10,nextpathID) VALUES (";
            for (LEN= LEN;LEN <= TOT_LEFT;LEN++)
            {
            if(LEN == 0)
            {
                QString I_D;
                I_D = QString("%1").arg(path_ID);
                qtts0.append(I_D);
                qtts0.append(",");
            }

            else
            {
                QString I_D;
                I_D = QString("'%1'").arg(PATH.value(LEN));
                qtts0.append(I_D);
                qtts0.append(",");
            }
            }
            QString I_D;
            I_D = QString("%1").arg(path_ID++);
            qtts0.append(I_D);
            qtts0.append(")");
            //qDebug() << qtts0;

            o = db.exec(qtts0);
        }
            int LEFT = TOT_LEFT - 11;
            QString qtts0 = "INSERT INTO pathInfoTable (pathID,nextID1,nextID2,nextID3,nextID4,nextID5,nextID6,nextID7,nextID8,nextID9,nextID10,nextpathID) VALUES (";
            for (LEN= LEN;LEN != 11;LEN++)
            {
            if(LEN == 0)
            {
                QString I_D;
                I_D = QString("%1").arg(path_ID);
                qtts0.append(I_D);
                qtts0.append(",");

                QString train_up = QString("UPDATE Trains SET pathID=%1 WHERE ID='%2'").arg(path_ID).arg(ui->trainBox->currentText());
                TRAIN = db.exec(train_up);
            }

            else
            {
                QString I_D;
                I_D = QString("'%1'").arg(PATH.value(LEN));
                qtts0.append(I_D);
                qtts0.append(",");
            }
            }
            QString I_D;
            path_ID++;
            I_D = QString("%1").arg(path_ID);
            qtts0.append(I_D);
            qtts0.append(")");
            //qDebug() << qtts0;

            o = db.exec(qtts0);

        //qDebug() <<"LEFT= "<< LEFT;
        if(LEFT <= 11) //Less than 10 points between source and destination
        {
            int LENL = 0;
            //qDebug() <<"LEN= "<< LEN <<"PATH.length()= "<<PATH.length();
            QString qtts0 = "INSERT INTO pathInfoTable (pathID,nextID1,nextID2,nextID3,nextID4,nextID5,nextID6,nextID7,nextID8,nextID9,nextID10,nextpathID) VALUES (";
            for (LEN = LEN;LEN != PATH.length();LEN++ )
            {
                if(LENL == 0)
                {
                    QString I_D;
                    I_D = QString("%1").arg(path_ID);
                    qtts0.append(I_D);
                    qtts0.append(",");
                }
                else
                {
                    QString I_D;
                    I_D = QString("'%1'").arg(PATH.value(LEN));
                    qtts0.append(I_D);
                    qtts0.append(",");
                }
                LENL++;
            }

            //qDebug() <<"LEN= "<< LEN;
                for (int C_ID = LEN;C_ID != 22; C_ID++)
                {
                    //qDebug() <<"C_ID= "<< C_ID;
                    qtts0.append("'NULL'");
                    qtts0.append(",");
                }
            QString I_D;
            I_D = QString("%1").arg(path_ID);
            qtts0.append(I_D);
            qtts0.append(")");
            //qDebug() << qtts0;

            o = db.exec(qtts0);
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
    }

    //qDebug() << sts1<< sts2<< sts3<<sts4<<sts5;

    QString ttps1 = QString("DELETE FROM pathInfoTable WHERE pathID=%1").arg(sts1);
    TRAIN2 = db.exec(ttps1);

    if(sts3 == sts4)
    {
        QString ttps1 = QString("DELETE FROM pathInfoTable WHERE pathID=%1").arg(sts4);
        Path2 = db.exec(ttps1);
    }
}

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
    }

    if(Sts1 == sts3)
    {
        QString ttps1 = QString("DELETE FROM pathInfoTable WHERE pathID=%1").arg(sts3);
        Path2 = db.exec(ttps1);
    }

    //qDebug() <<"string_ts= "<<string_ts;

    if(string_ts == "")
    {
        QString ttps1 = QString("DELETE FROM pathInfoTable WHERE nextID1='%1'").arg("NULL");
        Path2 = db.exec(ttps1);
    }
}

void MainWindow::check_sched()
{
//check team 2 SQL for track shutdowns. If any new ones, check schedules, trigger reroutes if necessary

//check occupancy data and adjust throttles accordingly,
    //If a schedule cannot be made up with throttle adjustments, trigger reroute
}
