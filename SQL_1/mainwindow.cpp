#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Send button

    connect(ui->sendButton,SIGNAL(clicked()),this,SLOT(executeSQL()));

    //Table1 Push Button  SWITCHES
    connect(ui->table1Button,SIGNAL(clicked()),this,SLOT(table1Ops()));

    //Table2 Push Button  TRAIN THROTTLE
    connect(ui->table2Button,SIGNAL(clicked()),this,SLOT(table2Ops()));

    //Table3 Push Button  TRAIN INFO
    connect(ui->table3Button,SIGNAL(clicked()),this,SLOT(table3Ops()));

    //Table4 Push Button  TRAIN PATH
    connect(ui->table4Button,SIGNAL(clicked()),this,SLOT(table4Ops()));



    //QSqlTableModel model;
    QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setHostName("localhost");
    db.setDatabaseName(":memory:");
    if (!db.open())
    {
        qDebug() << "Error";
    }

    db.open();

    QString query = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = 'ballers'";
    q = db.exec(query);


    // Create and populate table

    //TABLE
    q.exec("CREATE TABLE switchTable (switch TEXT, position TEXT);");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('1', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('2', 'Open');");
    q.exec("INSERT INTO switchTable VALUES ('3', 'Open');");
    q.exec("INSERT INTO switchTable VALUES ('4', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('5', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('6', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('7', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('8', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('9', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('10', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('11', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('12', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('13', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('14', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('15', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('16', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('17', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('18', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('19', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('20', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('21', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('22', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('23', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('24', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('25', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('26', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('27', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('28', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('29', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('30', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('31', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('32', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('33', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('34, 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('35', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('36', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('37', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('38', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('39', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('40', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('41', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('42', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('43', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('44', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('45', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('46', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('47', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('48', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('49', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('50', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('51', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('52', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('53', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('54', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('55', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('56', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('57', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('58', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('59', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('60', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('61', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('62', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('63', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('64', 'Open');");
    q.exec("INSERT INTO switchTable (switch, position) VALUES ('65', 'Open');");
    qDebug() << "************** Start **************";
    q.exec("SELECT * FROM switchTable;");
    while (q.next())
    qDebug() << "(" << q.value(0).toString()
    << ", " << q.value(1).toString() << ")";
    qDebug() << "************** End ***************";
    //


    model = new QSqlQueryModel;
    model->setQuery(q);
    view = new QTableView;
    view->setModel(model);
    view->show();

    //QString query = "SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = 'ballers'";

    //Table 2
    j = db.exec(query);

    j.exec("CREATE TABLE throttleTable (train TEXT, direction TEXT, throttle INT);");
    j.exec("INSERT INTO throttleTable (train, direction, throttle) VALUES ('Alabama Express', 'Forward', 1);");
    j.exec("INSERT INTO throttleTable (train, direction, throttle) VALUES ('Georgia Express', 'Forward', 2);");
    j.exec("INSERT INTO throttleTable VALUES ('Florida Express', 'Forward', 3);");
    j.exec("INSERT INTO throttleTable VALUES ('Mississippi Express', 'Backward', 4);");
    j.exec("INSERT INTO throttleTable (train, direction, throttle) VALUES ('Louisiana Express', 'Forward', 5);");
    j.exec("INSERT INTO throttleTable (train, direction, throttle) VALUES ('Tennessee Express', 'Backward', 6);");
    qDebug() << "************** Start **************";
    j.exec("SELECT * FROM throttleTable;");
    while (j.next())
    qDebug() << "(" << j.value(0).toString()
    << ", " << j.value(1).toString()
    << ", " << j.value(2).toInt() << ")";
    qDebug() << "************** End ***************";
    \
    model = new QSqlQueryModel;
    model->setQuery(j);
    view = new QTableView;
    view->setModel(model);
    view->show();


    //Table 3

    k = db.exec(query);

    k.exec("CREATE TABLE trainInfoTable (train TEXT, currentOcc TEXT, destination TEXT, next TEXT, path INT);");
    k.exec("INSERT INTO trainInfoTable (train, currentOcc, destination, next, path) VALUES ('Alabama Express', 'Full', 'Chicago', 'Denver', 1);");
    k.exec("INSERT INTO trainInfoTable (train, currentOcc, destination, next, path) VALUES ('Georgia Express', 'Full', 'Los Angeles', 'Denver', 2);");
    k.exec("INSERT INTO trainInfoTable VALUES ('Florida Express', 'FULL', 'Indianapolis', 'Seattle', 3);");
    k.exec("INSERT INTO trainInfoTable VALUES ('Mississippi Express', 'FULL','Raleigh', 'Birmingham', 4);");

    qDebug() << "************** Start **************";
    k.exec("SELECT * FROM trainInfoTable;");
    while (k.next())
    qDebug() << "(" << k.value(0).toString()
    << ", " << k.value(1).toString()
    << ", " << k.value(2).toString()
    << ", " << k.value(3).toString()
    << ", " << k.value(4).toInt() << ")";
    qDebug() << "************** End ***************";
    \
    model = new QSqlQueryModel;
    model->setQuery(k);
    view = new QTableView;
    view->setModel(model);
    view->show();


    //Table 4

    l = db.exec(query);

    l.exec("CREATE TABLE pathInfoTable (pathId INT, next2 TEXT, next11 TEXT);");
    l.exec("INSERT INTO pathInfoTable (pathId, next2, next11) VALUES (1, '3 5 9 11 31 37 50 52 44 48', '1 2 7 18 20 12 14 17 6 34');");
    l.exec("INSERT INTO pathInfoTable (pathId, next2, next11) VALUES (2, '1 7 10 8 21 27 16 19 13 9', '32 35 41 44 45 87 89 76 90 91');");
    l.exec("INSERT INTO pathInfoTable (pathId, next2, next11) VALUES (3, '100 95 77 74 49 79 66 53 57 58', ' 62 63 64 65 68 69 70);");
    l.exec("INSERT INTO pathInfoTable (pathId, next2, next11) VALUES (4,'3 5 9 11 31 37 50 52 44 48', '1 2 7 18 27 12 14 17 6 34);");
    l.exec("INSERT INTO pathInfoTable (pathId, next2, next11) VALUES (5, '3 5 9 11 31 37 50 52 44 48', '1 2 7 18 29 12 14 17 6 34);");
    l.exec("INSERT INTO pathInfoTable (pathId, next2, next11) VALUES (6, '3 5 9 11 31 37 50 52 44 48', '1 2 7 18 30 12 14 17 6 34);");
    qDebug() << "************** Start **************";
    l.exec("SELECT * FROM pathInfoTable;");
    while (l.next())
    qDebug() << "(" << l.value(0).toInt()
    << ", " << l.value(1).toString()
    << ", " << l.value(2).toString() << ")";
    qDebug() << "************** End ***************";
    \
    model = new QSqlQueryModel;
    model->setQuery(l);
    view = new QTableView;
    view->setModel(model);
    view->show();




}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initializeModel(QSqlTableModel *model)
{
    qDebug() << "Initialize happened";
    model->setTable("person");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("First name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Last name"));
}

QTableView *MainWindow::createView(QSqlTableModel *model, const QString &title)
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle(title);
    view->show();
    return view;
}


void MainWindow::executeSQL()
{

    q.exec(ui->lineEdit->text());
    while (q.next())
    qDebug() << "(" << q.value(0).toInt() << ", " << q.value(1).toString()<< ", " << q.value(2).toString() << ")";
    qDebug() << "************** End ***************";

    model->setQuery(q);
    view = new QTableView;
    view->setModel(model);
    view->show();

}

void MainWindow::table1Ops()
{
    qDebug() << "Table 1 Ops";
    model->setQuery(q);
    view = new QTableView;
    view->setModel(model);
    view->show();

}

void MainWindow::table2Ops()
{

    model->setQuery(j);
    view = new QTableView;
    view->setModel(model);
    view->show();
}

void MainWindow::table3Ops()
{
    model->setQuery(k);
    view = new QTableView;
    view->setModel(model);
    view->show();
}

void MainWindow::table4Ops()
{
    model->setQuery(l);
    view = new QTableView;
    view->setModel(model);
    view->show();

}
