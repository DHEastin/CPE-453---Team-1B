#include "train_dialog.h"
#include "ui_train_dialog.h"
#include "mainwindow.h"

Train_Dialog::Train_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Train_Dialog)
{
    ui->setupUi(this);
}

Train_Dialog::~Train_Dialog()
{
    delete ui;
}
