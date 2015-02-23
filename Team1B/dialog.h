#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QString>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    int start; //global variable for Starting Point
    int dest; //global variable for Destination
    ~Dialog();

private:
    Ui::Dialog *ui;

private slots:
    void dij_mainprogram(); // Main program in dij_algorithm.cpp
    void RESETVALUE();
    void train_info();
    void switch_info();
    void path_info();
    void trottle_info();
    void sql_query();
    void create_sqltables();

};

#endif // DIALOG_H
