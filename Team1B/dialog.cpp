#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);    

    ui->sourceBox->setRange(0,46);
    ui->destBox->setRange(0,46);
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(dij_mainprogram()));
    connect(ui->clearButton,SIGNAL(clicked()),this,SLOT(RESETVALUE()));
    connect(ui->clearButton,SIGNAL(clicked()),this,SLOT(RESETVALUE()));
    connect(ui->clearButton,SIGNAL(clicked()),ui->distanceEdit,SLOT(clear()));
    connect(ui->clearButton,SIGNAL(clicked()),ui->pathEdit,SLOT(clear()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::RESETVALUE()
{
    ui->sourceBox->setValue(0);
    ui->destBox->setValue(0);
}
