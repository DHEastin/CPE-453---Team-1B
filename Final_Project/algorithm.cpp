#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <set>
#include <limits>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

QString TEST;

//MUST BE HERE TO MAKE THE ALGORITHM WORK
double inf = std::numeric_limits<double>::infinity();

typedef int vertex_t;
typedef double weight_t;

//String values used in output to ui
QString s;
QString s2;

struct edge {
    vertex_t target;
    weight_t weight;
    edge(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) { }
};

typedef std::map<vertex_t, std::list<edge> > adjacency_map_t;
adjacency_map_t adjacency_map;

template <typename T1, typename T2>
struct pair_first_less
{
    bool operator()(std::pair<T1,T2> p1, std::pair<T1,T2> p2)
    {
        return p1.first < p2.first;
    }
};

void DijkstraComputePaths(vertex_t source,
                          adjacency_map_t& adjacency_map,
                          std::map<vertex_t, weight_t>& min_distance,
                          std::map<vertex_t, vertex_t>& previous)
{
    for (adjacency_map_t::iterator vertex_iter = adjacency_map.begin();
         vertex_iter != adjacency_map.end();
         vertex_iter++)
    {
        vertex_t v = vertex_iter->first;
        min_distance[v] = inf;
    }
    min_distance[source] = 0;
    std::set< std::pair<weight_t, vertex_t>,
              pair_first_less<weight_t, vertex_t> > vertex_queue;
    for (adjacency_map_t::iterator vertex_iter = adjacency_map.begin();
         vertex_iter != adjacency_map.end();
         vertex_iter++)
    {
        vertex_t v = vertex_iter->first;
        vertex_queue.insert(std::pair<weight_t, vertex_t>(min_distance[v], v));
    }

    while (!vertex_queue.empty()) {
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
        for (std::list<edge>::iterator edge_iter = adjacency_map[u].begin();
             edge_iter != adjacency_map[u].end();
             edge_iter++)
        {
            vertex_t v = edge_iter->target;
            weight_t weight = edge_iter->weight;
            weight_t distance_through_u = min_distance[u] + weight;
        if (distance_through_u < min_distance[v]) {
            vertex_queue.erase(std::pair<weight_t, vertex_t>(min_distance[v], v));

            min_distance[v] = distance_through_u;
            previous[v] = u;
            vertex_queue.insert(std::pair<weight_t, vertex_t>(min_distance[v], v));
        }
        }
    }
}

std::list<vertex_t> DijkstraGetShortestPathTo(
    vertex_t target, std::map<vertex_t, vertex_t>& previous)
{
    std::list<vertex_t> path;
    std::map<vertex_t, vertex_t>::iterator prev;
    vertex_t vertex = target;
    path.push_front(vertex);
    while((prev = previous.find(vertex)) != previous.end())
    {
        vertex = prev->second;
        path.push_front(vertex);
    }
    return path;
}

void MainWindow::dij_mainprogram() //Main Program
{
     adjacency_map.clear(); //Clears ajacency_map
     CHECKER = 0;

  //Reads track ids and names for showing the path
    /*-------------------------------------------------------------------*/
    QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
    nn = db.exec(t);

    for(;nn.next() == 1;) //If it is 1 it contains data
    {
        QString ss4 = nn.value(1).toString();
        vertex_names.push_back(ss4.toStdString());
    }
    /*-------------------------------------------------------------------*/

    //Reads data for each track and how its connected along with its weight
    QString t2 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    INFO = db.exec(t2);

    //For each swich in trackInfoTable execute the following:
    /*-------------------------------------------------------------------*/
    for(;INFO.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = INFO.value(0).toInt();
    int ss2 = INFO.value(1).toInt();
    int ss3 = INFO.value(2).toInt();
    adjacency_map[ss1].push_back(edge(ss2,  ss3));
    }
    /*-------------------------------------------------------------------*/

    dij_main();//Call algorithm function

    if (CHECKER == 1)//Use for checking purposes
    {
    adjacency_map.clear(); //Clears ajacency_map must be done to allow opposite direction of travel
    CHECKER = 0;
    //Reads data for each track and how its connected along with its weight
    QString t3 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    INFO2 = db.exec(t3);

    //For each swich in trackInfoTable execute the following:
    /*-------------------------------------------------------------------*/
    for(;INFO2.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = INFO2.value(0).toInt();
    int ss2 = INFO2.value(1).toInt();
    int ss3 = INFO2.value(2).toInt();
    adjacency_map[ss2].push_back(edge(ss1,  ss3));
    //qDebug() <<ss2<<",{"<<ss1<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/

    dij_main();//Call algorithm function

    if(CHECKER == 1)//Use for checking purposes
    {
        count = 0;
        CHECKER = 0;
        s = QString("Distance from vertex %1 to %2 is: %3").arg(start).arg(dest).arg("NULL-No Path Exists!");
        //ui->distanceEdit->setText(s);
        std::cout << "No Path Exists or switch is not correctly switched!"<< std::endl;
        s2 = QString("Path: No Path Exists or switch is not correctly switched!");
        //ui->pathEdit->setText(s2);
    }
    }
}

//Dialog:: was added to iterface with Qt dialog.cpp
void MainWindow::dij_main()
{
    count = 0;//count is used to output only one line of paths/distance

    QString ts1 = QString("SELECT ID,START,Direction,Destination from %1 WHERE ID='%2").arg("Trains").arg(ui->trainBox->currentText());
    TRAIN = db.exec(ts1);

    QString START;
    QString DEST;

    /*-------------------------------------------------------------------*/
    for(;TRAIN.next() == 1;) //If it is 1 it contains data
    {
    START = TRAIN.value(1).toString();
    DEST = TRAIN.value(3).toString();
    }


    QString t3 = QString("SELECT trackID,trackNAME from %1").arg("tracklistingTable");
    mm = db.exec(t3);

    //For each swich in trackInfoTable execute the following:
    /*-------------------------------------------------------------------*/
    for(;mm.next() == 1;) //If it is 1 it contains data
    {
    int sts1 = mm.value(0).toInt();
    QString sts2 = mm.value(1).toString();

    if (START == sts2)
    {
        start = sts1;
    }
    if (DEST == sts2)
    {
        dest = sts1;
    }

    }
    /*-------------------------------------------------------------------*/

    std::map<vertex_t, weight_t> min_distance;
    std::map<vertex_t, vertex_t> previous;

    //start is the Starting node
    DijkstraComputePaths(start, adjacency_map, min_distance, previous);
    for (adjacency_map_t::iterator vertex_iter = adjacency_map.begin();
         vertex_iter != adjacency_map.end();
         vertex_iter++)
    {
        vertex_t v = vertex_iter->first;

        if (v == dest)
        {
           if(count == 0)
           {
            if (min_distance[v] == inf)
            {
            CHECKER = 1;
            }
            else
            {
            CHECKER = 0;
            s = QString("Distance from vertex %1 to %2 is: %3").arg(START).arg(DEST).arg(min_distance[v]);
            //ui->distanceEdit->clear();
            //ui->distanceEdit->setText(s);
            std::cout << "Distance to " << vertex_names[v] << ": " << min_distance[v] << std::endl;
            std::list<vertex_t> path =
                DijkstraGetShortestPathTo(v, previous);
            std::list<vertex_t>::iterator path_iter = path.begin();
            s2 = QString("Path: ");
            //ui->pathEdit->clear();
            //ui->pathEdit->setText(s2);
            std::cout << "Path: ";
            for( ; path_iter != path.end(); path_iter++)
            {
                //Below statements are used to add arrows between destinations and no arrow on last node
                QString s3;
                if(*path_iter == dest)
                {
                    s3 = QString("%1 ").arg(QString::fromStdString(vertex_names[*path_iter]));
                    std::cout << vertex_names[*path_iter] << " ";
                }
                else
                {
                s3 = QString("%1 -> ").arg(QString::fromStdString(vertex_names[*path_iter]));
                std::cout << vertex_names[*path_iter] << " ->";
                }
                s2.append(s3);//Appends text to path showing the full path
                //ui->pathEdit->setText(s2);
            }
            std::cout << std::endl;
            }
            std::cout << std::endl;
            }
           count++;//Increments count by 1 to prevent more than one line of outputs
        }
    }
}
