#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void train_dia();
    void edit_train_dia();
    void del_train_dia();
    void sched_dia();
    void make_map();
    void check_sched();
};

#endif // MAINWINDOW_H
