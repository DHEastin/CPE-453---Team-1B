#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QString>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QTableView>
#include <QVector>
#include <QListWidget>
#include <QComboBox>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int start; //global variable for Starting Point
    int dest; //global variable for Destination
    QString destpick;
    QString startpick;
    int CHECKER;//algorithm file variable
    int count;//algorithm file variable
    int ii;
    int COL0;
    int COL1;
    int COL2;
    int COL3;
    int NumofConn;
    void initializeModel(QSqlTableModel *model);
     QTableView *createView(QSqlTableModel *model, const QString &title = "");
     QSqlTableModel *table1;
     QSqlQuery q,j,k,l,m,n,q1,k1,m1,n1,o,p,INFO,INFO2,nn,mm,qq;
     QSqlQueryModel *model;
     QTableView *view;
     QSqlDatabase db,rdb;
     std::vector<std::string> vertex_names;
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
    void dij_main(); // Main program in dij_algorithm.cpp
    void dij_mainprogram(); // Algorithm part of program in dij_algorithm.cpp
    void sql_query();
    void create_sqltables();
    void sqlserver_connect();
    void VIEWTABLE();
};

#endif // MAINWINDOW_H
