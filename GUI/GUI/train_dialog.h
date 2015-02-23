#ifndef TRAIN_DIALOG_H
#define TRAIN_DIALOG_H

#include <QDialog>

namespace Ui {
class Train_Dialog;
}

class Train_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Train_Dialog(QWidget *parent = 0);
    ~Train_Dialog();

private:
    Ui::Train_Dialog *ui;
};

#endif // TRAIN_DIALOG_H
