#include "schedule_dialog.h"
#include "ui_schedule_dialog.h"

Schedule_Dialog::Schedule_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Schedule_Dialog)
{
    ui->setupUi(this);
}

Schedule_Dialog::~Schedule_Dialog()
{
    delete ui;
}
