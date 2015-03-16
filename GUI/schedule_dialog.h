#ifndef SCHEDULE_DIALOG_H
#define SCHEDULE_DIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Schedule_Dialog;
}

class Schedule_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Schedule_Dialog(QWidget *parent = 0);
    QSqlQuery n;
    QSqlDatabase db,rdb;
    QString DESTINATION;
    ~Schedule_Dialog();

private:
    Ui::Schedule_Dialog *ui;

private slots:
    void REMOTE();
    void LOCAL();

public slots:
    void Save_Schedule();
};

#endif // SCHEDULE_DIALOG_H
