#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QString>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QTableView>
#include <QVector>
#include <QListWidget>
#include <QComboBox>

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
    ~Dialog();

private:
    Ui::Dialog *ui;

private slots:
    void dij_main(); // Main program in dij_algorithm.cpp
    void dij_mainprogram(); // Algorithm part of program in dij_algorithm.cpp
    void sql_query();
    void create_sqltables();
    void sqlserver_connect();
    void VIEWTABLE();
    void SWAP();
};

#endif // DIALOG_H
