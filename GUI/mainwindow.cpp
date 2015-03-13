#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "schedule_dialog.h"
#include "train_dialog.h"
#include "path_map.h"
#include <QDialog>
#include <QTimer>
#include <QDebug>

Path_Map schedule_map = Path_Map();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //create_sqltables();

    QTimer* persistence = new QTimer();

    QObject::connect(persistence, SIGNAL(timeout()), this, SLOT(check_sched()));
    QObject::connect(ui->actionAdd_Train, SIGNAL(triggered()), this, SLOT(train_dia()));
    QObject::connect(ui->schedule, SIGNAL(pressed()), this, SLOT(sched_dia()));
    QObject::connect(ui->actionLoad_Track, SIGNAL(triggered()), this, SLOT(make_map()));

    persistence->start(200);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::train_dia()
{
    Train_Dialog dia;
    dia.exec();
}

void MainWindow::edit_train_dia()
{
    //Edit_Train_Dialog dia;
    //dia.exec();
}

void MainWindow::del_train_dia()
{
    //Del_Train_Dialog dia;
    //dia.exec();
}

void MainWindow::sched_dia()
{
    MainWindow::create_sqltables();
    Schedule_Dialog dia;
    dia.exec();
}

void MainWindow::make_map()
{
    //query SQL and build track. Delete entire map, and reload from scratch when this slot is used

    //check and redo schedules if needed
}

void MainWindow::check_sched()
{
//check team 2 SQL for track shutdowns. If any new ones, check schedules, trigger reroutes if necessary

//check occupancy data and adjust throttles accordingly,
    //If a schedule cannot be made up with throttle adjustments, trigger reroute
}

