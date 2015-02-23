#ifndef SCHEDULE_DIALOG_H
#define SCHEDULE_DIALOG_H

#include <QDialog>

namespace Ui {
class Schedule_Dialog;
}

class Schedule_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Schedule_Dialog(QWidget *parent = 0);
    ~Schedule_Dialog();

private:
    Ui::Schedule_Dialog *ui;
};

#endif // SCHEDULE_DIALOG_H
