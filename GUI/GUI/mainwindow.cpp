#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "schedule_dialog.h"
#include "train_dialog.h"
#include <QDialog>
//#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->actionAdd_Train, SIGNAL(triggered()), this, SLOT(train_dia()));
    QObject::connect(ui->schedule, SIGNAL(pressed()), this, SLOT(sched_dia()));
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
    Schedule_Dialog dia;
    dia.exec();
}
