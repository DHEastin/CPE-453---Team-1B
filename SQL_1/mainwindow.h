#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QTableView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initializeModel(QSqlTableModel *model);
     QTableView *createView(QSqlTableModel *model, const QString &title = "");
     QSqlTableModel *table1;
     QSqlQuery q,j,k,l;
     QSqlQueryModel *model;
     QTableView *view;
public slots:
     void executeSQL();
     void table1Ops();
     void table2Ops();
     void table3Ops();
     void table4Ops();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
