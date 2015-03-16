#ifndef TRAIN_DIALOG_H
#define TRAIN_DIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Train_Dialog;
}

class Train_Dialog : public QDialog

{
    Q_OBJECT

public:
    explicit Train_Dialog(QWidget *parent = 0);
     QSqlQuery n,o,p;
     QSqlDatabase db,rdb;
     //QString DIRECTION;
     //static QString TrainID;
     struct ID
     {
         QString TrainID;
         QString DIRECTION;
     };
    ~Train_Dialog();

private:
    Ui::Train_Dialog *ui;

private slots:
    void REMOTE();
    void LOCAL();
    void Forward_Function();
    void Backward_Function();

public slots:
    void SaveTrainID();
};

#endif // TRAIN_DIALOG_H
