#include "train_dialog.h"
#include "ui_train_dialog.h"
#include "mainwindow.h"

Train_Dialog::Train_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Train_Dialog)
{
    ui->setupUi(this);

    //ui->startBox->currentText();

    QString ts3 = QString("SELECT trackID,trackNAME from %1").arg("tracklistingTable");
    n = db1.exec(ts3);

    /*-------------------------------------------------------------------*/
    for(;n.next() == 1;) //If it is 1 it contains data
    {
    //int sts1 = n.value(0).toInt();
    QString sts2 = n.value(1).toString();
    ui->startBox->addItem(sts2);
    }
}

Train_Dialog::~Train_Dialog()
{
    delete ui;
}

