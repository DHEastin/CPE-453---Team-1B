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
    void initializeModel(QSqlTableModel *model);
     QTableView *createView(QSqlTableModel *model, const QString &title = "");
     QSqlTableModel *table1;
     QSqlQuery q,j,k,l,m,n;
     QSqlQueryModel *model;
     QTableView *view;
     QSqlDatabase db;
     std::vector<std::string> vertex_names;
    ~Dialog();

private:
    Ui::Dialog *ui;

private slots:
    void dij_mainprogram(); // Main program in dij_algorithm.cpp
    void RESETVALUE();
    void train_info();
    void switch_info();
    void path_info();
    void throttle_info();
    void track_info();
    void track_listing();
    void sql_query();
    void create_sqltables();
    void sqlserver_connect();
    void swapNUM();

};

#endif // DIALOG_H
