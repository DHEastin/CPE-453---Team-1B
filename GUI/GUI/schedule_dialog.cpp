#include "schedule_dialog.h"
#include "ui_schedule_dialog.h"
#include "QDebug"

Schedule_Dialog::Schedule_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Schedule_Dialog)
{
    ui->setupUi(this);

    //ui->destBox->currentText();

    QString ts3 = QString("SELECT trackID,trackNAME from %1").arg("tracklistingTable");
    db.open();
    n = db.exec(ts3);
    qDebug() << "Testing";

    /*-------------------------------------------------------------------*/
    for(;n.next() == 1;) //If it is 1 it contains data
    {
    //int sts1 = n.value(0).toInt();
    QString sts2 = n.value(1).toString();
    ui->destBox->addItem(sts2);
    }
}

Schedule_Dialog::~Schedule_Dialog()
{
    delete ui;
}
