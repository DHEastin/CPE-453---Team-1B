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
#include <QDir>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QFile>
#include <QtSql>
#include <QStandardItemModel>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString ID;
    QString Start;
    QString direction;
    int path_ID;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db,rdb;
    int CHECKER;
    void initializeModel(QSqlTableModel *model);
    QTableView *createView(QSqlTableModel *model, const QString &title = "");
    QSqlTableModel *table1;
    QSqlQueryModel *qmodel;
    QSqlQuery q,j,k,l,m,n,q1,k1,m1,n1,o,p,INFO,INFO2,nn,mm,qq,TRAIN,TRAIN2,LOAD,Path,Path2;
    std::vector<std::string> vertex_names;
    int count;
    int start; //global variable for Starting Point
    int dest; //global variable for Destination
    int ii;
    QTableView *view;
    int COL0;
    int COL1;
    int COL2;
    int COL3;
    int NumofConn;
    QStringList items;
    bool ok;
    QString Destination;
    int isSave;
    QSqlTableModel* tmodel;
    QStandardItemModel* imodel;
    QString START;
    QString DEST;
    QStringList PATH;
    bool ERROR;

private slots:
    void sql_query();
    void create_sqltables();
    void Add_Train();
    void Edit_Train();
    void Delete_Train();
    void Load_State();
    void Save_State();
    void Schedule();
    void Set_Schedule();
    void Train_Table();
    void dij_main();
    void dij_mainprogram();
    void DS_Connectivity_Table();
    void Tracklisting_Table();
    void Trackinfo_Table();
    void Update_ScheduleTable();
    void Pathinfo_Table();
    void check_sched();
};

#endif // MAINWINDOW_H
