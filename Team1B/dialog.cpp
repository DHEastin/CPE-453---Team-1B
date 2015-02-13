#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->sourceBox->setRange(0,6);
    ui->destBox->setRange(0,6);
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(dij_mainprogram()));
}

Dialog::~Dialog()
{
    delete ui;
}
