#ifndef TRAIN_DIALOG_H
#define TRAIN_DIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>

namespace Ui {
class Train_Dialog;
}

class Train_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Train_Dialog(QWidget *parent = 0);
     QSqlQuery n;
     QSqlDatabase db1;
    ~Train_Dialog();

private:
    Ui::Train_Dialog *ui;
};

#endif // TRAIN_DIALOG_H
