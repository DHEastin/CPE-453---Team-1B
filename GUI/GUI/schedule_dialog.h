#ifndef SCHEDULE_DIALOG_H
#define SCHEDULE_DIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>

namespace Ui {
class Schedule_Dialog;
}

class Schedule_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Schedule_Dialog(QWidget *parent = 0);
    QSqlQuery n;
    QSqlDatabase db;
    ~Schedule_Dialog();

private:
    Ui::Schedule_Dialog *ui;
};

#endif // SCHEDULE_DIALOG_H
