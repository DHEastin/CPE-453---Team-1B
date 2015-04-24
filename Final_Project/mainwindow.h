#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QString>
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
#include <QStandardItemModel>
#include <QFileDialog>
#include <QtSql>

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
    int ID;
    QString Start;
    QString direction;
    int path_ID;
    QSqlDatabase db,rdb; //,LOCONET;
    bool overrideStatus;
    int CHECKER;
    void initializeModel(QSqlTableModel *model);
    QTableView *createView(QSqlTableModel *model, const QString &title = "");
    QSqlTableModel *table1;
    QSqlQueryModel *qmodel;
    QSqlQuery overrideCheck,q,j,k,l,m,n,q1,k1,m1,n1,kk,o,p,INFO,INFO2,nn,mm,qq,TRAIN,TRAIN2,LOAD,Path,Path2,l1,l2,l3,l4,l5,r1,r2,r3,r4,r5,Train1,Train2,Train3,TrainSW1,TrainSW2,TrainSW3,TrainSW4,TrainSW5,runSchedQuery,runSchedQuery1,runSchedQuery2,runSchedQuery3;
    std::vector<std::string> vertex_names;
    int count;
    int count2;
    int start; //global variable for Starting Point
    int dest; //global variable for Destination
    int ii;
    QTableView *view,*view2;
    int COL0;
    int COL1;
    int COL2;
    int COL3;
    int NumofConn;
    QStringList items,trains;
    bool ok;
    QString Destination;
    int isSave;
    QSqlTableModel *tmodel,*tmodel2,*tmodel3;
    QStandardItemModel* imodel;
    QString START;
    QString DEST;
    QStringList PATH;
    bool ERROR;
    int a;
    int MIN_DISTANCE;
    QString SwitchDIR;
    QString CLOCK,PATH_DIR;
    //OPTIONS
    bool Testing_path;

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
    void dij_mainprogram_copy();
    void DS_Connectivity_Table();
    void Tracklisting_Table();
    void Trackinfo_Table();
    void Trackinfo2_Table();
    void Update_ScheduleTable();
    void Pathinfo_Table();
    void DEL_OLD_PATH();
    void DEL_ALL_OLD_PATH();
    void Check_Path_Trains();
    void check_sched();
    void Schedule_train_info();
    void Scheduled_routes();
    void Scheduled_train_info();
    void Throttle_req();
    void Switch_req();
    void Switch_info();
    void Trains1_Table();
    void Trains2_Table();
    void Trains3_Table();
    void Trains4_Table();
    void Trains5_Table();
};

#endif // MAINWINDOW_H
