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

/*-------------------------------------------------------------------------------------------------------------*/
//dij_mainprogram is called from mainwindow and is used to determine the path from source to destination
//dij_main is called to determine the desired path
//This will flip the adjacency_map to allow for traveling both directions on track
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::dij_mainprogram()
{
    a = 0;
    adjacency_map.clear(); //Clears ajacency_map
    CHECKER = 0;

    //Add Switch Positions switch -> openPOS
    QString tqtps1 = QString("SELECT switch,position,straight,openPOS,closedPOS FROM switchInfoTable");
    p = db.exec(tqtps1);

    for(;p.next() == 1;) //If it is 1 it contains data
    {
        int ss1 = p.value(0).toInt();
        int ss4 = p.value(3).toInt();
        adjacency_map[ss1].push_back(edge(ss4,  5));
     }


   //Reads track ids and names for showing the path
    /*-------------------------------------------------------------------*/
    QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
    n = db.exec(t);

    for(;n.next() == 1;) //If it is 1 it contains data
    {
        QString ss4 = n.value(1).toString();
        vertex_names.push_back(ss4.toStdString());
    }
    /*-------------------------------------------------------------------*/

    //Reads data for each track and how its connected along with its weight
    QString t2 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    qq = db.exec(t2);

    /*-------------------------------------------------------------------*/
    for(;qq.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = qq.value(0).toInt();
    int ss2 = qq.value(1).toInt();
    int ss3 = qq.value(2).toInt();
    adjacency_map[ss1].push_back(edge(ss2,  ss3));
    //qDebug() <<ss1<<",{"<<ss2<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/

    qDebug() <<"BFC"<< CHECKER;
    dij_main();//Call algorithm function
    qDebug() <<"AFC"<< CHECKER;

    //Flip if inf distance
    if (CHECKER == 1)//Use for checking purposes
    {
    adjacency_map.clear(); //Clears ajacency_map must be done to allow opposite direction of travel
    //CHECKER = 0;

    //Add Switch Positions openPOS -> switch
    QString tqtps1 = QString("SELECT switch,position,straight,openPOS,closedPOS FROM switchInfoTable");
    p = db.exec(tqtps1);

    for(;p.next() == 1;) //If it is 1 it contains data
    {
        int ss1 = p.value(0).toInt();
        int ss4 = p.value(4).toInt();
        adjacency_map[ss1].push_back(edge(ss4,  5));
    }

    //Reads track ids and names for showing the path
      /*-------------------------------------------------------------------*/
      QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
      n = db.exec(t);

      for(;n.next() == 1;) //If it is 1 it contains data
      {
          QString ss4 = n.value(1).toString();
          vertex_names.push_back(ss4.toStdString());
      }
      /*-------------------------------------------------------------------*/

    //Reads data for each track and how its connected along with its weight
    QString t3 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    qq = db.exec(t3);

    /*-------------------------------------------------------------------*/
    for(;qq.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = qq.value(0).toInt();
    int ss2 = qq.value(1).toInt();
    int ss3 = qq.value(2).toInt();
    adjacency_map[ss2].push_back(edge(ss1,  ss3));
    //qDebug() <<ss2<<",{"<<ss1<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/

    qDebug() <<"BFC"<< CHECKER;
    dij_main();//Call algorithm function
    qDebug() <<"AFC"<< CHECKER;
    qDebug() <<"End 1st Half of Algorithm";
    }

    //*-------------------------------------------------------------------------------------------------------------------------------------------------
    //*-------------------------------------------------------------------------------------------------------------------------------------------------
    //*-------------------------------------------------------------------------------------------------------------------------------------------------

    //Start of Opening Switches
    if(CHECKER == 1)//Use for checking purposes
    {
        qDebug()<<"Initilize 2nd Part of Algorithm";
        adjacency_map.clear(); //Clears ajacency_map
        CHECKER = 0;

         //Add Switch Positions switch -> closedPOS
         QString tqtps1 = QString("SELECT switch,position,straight,openPOS,closedPOS FROM switchInfoTable");
         p = db.exec(tqtps1);

         for(;p.next() == 1;) //If it is 1 it contains data
         {
             int ss1 = p.value(0).toInt();
             //int ss2 = p.value(1).toInt();
             //int ss3 = p.value(2).toInt();
             //int ss4 = p.value(3).toInt();
             int ss5 = p.value(3).toInt();
             adjacency_map[ss5].push_back(edge(ss1,  5));
         }


  //Reads track ids and names for showing the path
    /*-------------------------------------------------------------------*/
    QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
    n = db.exec(t);

    for(;n.next() == 1;) //If it is 1 it contains data
    {
        QString ss4 = n.value(1).toString();
        vertex_names.push_back(ss4.toStdString());
    }
    /*-------------------------------------------------------------------*/

    //Reads data for each track and how its connected along with its weight
    QString t2 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    qq = db.exec(t2);

    /*-------------------------------------------------------------------*/
    for(;qq.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = qq.value(0).toInt();
    int ss2 = qq.value(1).toInt();
    int ss3 = qq.value(2).toInt();
    adjacency_map[ss1].push_back(edge(ss2,  ss3));
    //qDebug() <<ss1<<",{"<<ss2<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/
    qDebug() <<"BFC"<< CHECKER;
    dij_main();//Call algorithm function
    qDebug() <<"AFC"<< CHECKER;
    }

    if (CHECKER == 1)//Use for checking purposes
    {
    adjacency_map.clear(); //Clears ajacency_map must be done to allow opposite direction of travel

    //Add Switch Positions closedPOS -> switch
    QString tqtps1 = QString("SELECT switch,position,straight,openPOS,closedPOS FROM switchInfoTable");
    p = db.exec(tqtps1);
    for(;p.next() == 1;) //If it is 1 it contains data
    {
        int ss1 = p.value(0).toInt();
        //int ss2 = p.value(1).toInt();
        //int ss3 = p.value(2).toInt();
        //int ss4 = p.value(3).toInt();
        int ss5 = p.value(4).toInt();
        adjacency_map[ss5].push_back(edge(ss1,  5));
    }

    //Reads track ids and names for showing the path
      /*-------------------------------------------------------------------*/
      QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
      n = db.exec(t);

      for(;n.next() == 1;) //If it is 1 it contains data
      {
          QString ss4 = n.value(1).toString();
          vertex_names.push_back(ss4.toStdString());
      }
      /*-------------------------------------------------------------------*/


    //Reads data for each track and how its connected along with its weight
    QString t3 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    qq = db.exec(t3);

    //For each switch in trackInfoTable execute the following:
    /*-------------------------------------------------------------------*/
    for(;qq.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = qq.value(0).toInt();
    int ss2 = qq.value(1).toInt();
    int ss3 = qq.value(2).toInt();
    adjacency_map[ss2].push_back(edge(ss1,  ss3));
    //qDebug() <<ss2<<",{"<<ss1<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/
    qDebug() <<"BFC"<< CHECKER;
    dij_main();//Call algorithm function
    qDebug() <<"AFC"<< CHECKER;
    }

    //*-------------------------------------------------------------------------------------------------------------------------------------------------
    //*-------------------------------------------------------------------------------------------------------------------------------------------------
    //*-------------------------------------------------------------------------------------------------------------------------------------------------
}

void MainWindow::dij_mainprogram_copy() //Main Program
{
    a = 0;
    adjacency_map.clear(); //Clears ajacency_map
    CHECKER = 0;

    //Add Switch Positions switch -> openPOS
    QString tqtps1 = QString("SELECT switch,position,straight,openPOS,closedPOS FROM switchInfoTable");
    p = db.exec(tqtps1);

    for(;p.next() == 1;) //If it is 1 it contains data
    {
        int ss1 = p.value(0).toInt();
        int ss4 = p.value(3).toInt();
        adjacency_map[ss1].push_back(edge(ss4,  5));
     }


   //Reads track ids and names for showing the path
    /*-------------------------------------------------------------------*/
    QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
    n = db.exec(t);

    for(;n.next() == 1;) //If it is 1 it contains data
    {
        QString ss4 = n.value(1).toString();
        vertex_names.push_back(ss4.toStdString());
    }
    /*-------------------------------------------------------------------*/

    //Reads data for each track and how its connected along with its weight
    QString t2 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    qq = db.exec(t2);

    /*-------------------------------------------------------------------*/
    for(;qq.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = qq.value(0).toInt();
    int ss2 = qq.value(1).toInt();
    int ss3 = qq.value(2).toInt();
    adjacency_map[ss1].push_back(edge(ss2,  ss3));
    //qDebug() <<ss1<<",{"<<ss2<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/

    qDebug() <<"BFC"<< CHECKER;
    dij_main();//Call algorithm function
    qDebug() <<"AFC"<< CHECKER;

    //Flip if inf distance
    if (CHECKER == 1)//Use for checking purposes
    {
    adjacency_map.clear(); //Clears ajacency_map must be done to allow opposite direction of travel
    //CHECKER = 0;

    //Add Switch Positions openPOS -> switch
    QString tqtps1 = QString("SELECT switch,position,straight,openPOS,closedPOS FROM switchInfoTable");
    p = db.exec(tqtps1);

    for(;p.next() == 1;) //If it is 1 it contains data
    {
        int ss1 = p.value(0).toInt();
        int ss4 = p.value(3).toInt();
        adjacency_map[ss1].push_back(edge(ss4,  5));
    }

    //Reads track ids and names for showing the path
      /*-------------------------------------------------------------------*/
      QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
      n = db.exec(t);

      for(;n.next() == 1;) //If it is 1 it contains data
      {
          QString ss4 = n.value(1).toString();
          vertex_names.push_back(ss4.toStdString());
      }
      /*-------------------------------------------------------------------*/

    //Reads data for each track and how its connected along with its weight
    QString t3 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    qq = db.exec(t3);

    /*-------------------------------------------------------------------*/
    for(;qq.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = qq.value(0).toInt();
    int ss2 = qq.value(1).toInt();
    int ss3 = qq.value(2).toInt();
    adjacency_map[ss2].push_back(edge(ss1,  ss3));
    //qDebug() <<ss2<<",{"<<ss1<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/

    qDebug() <<"BFC"<< CHECKER;
    dij_main();//Call algorithm function
    qDebug() <<"AFC"<< CHECKER;
    qDebug() <<"End 1st Half of Algorithm";
    }

    //*-------------------------------------------------------------------------------------------------------------------------------------------------
    //*-------------------------------------------------------------------------------------------------------------------------------------------------
    //*-------------------------------------------------------------------------------------------------------------------------------------------------

    //Start of Opening Switches
    if(CHECKER == 1)//Use for checking purposes
    {
        qDebug()<<"Initilize 2nd Part of Algorithm";
        adjacency_map.clear(); //Clears ajacency_map
        CHECKER = 0;

         //Add Switch Positions switch -> closedPOS
         QString tqtps1 = QString("SELECT switch,position,straight,openPOS,closedPOS FROM switchInfoTable");
         p = db.exec(tqtps1);

         for(;p.next() == 1;) //If it is 1 it contains data
         {
             int ss1 = p.value(0).toInt();
             //int ss2 = p.value(1).toInt();
             //int ss3 = p.value(2).toInt();
             //int ss4 = p.value(3).toInt();
             int ss5 = p.value(4).toInt();
             adjacency_map[ss1].push_back(edge(ss5,  5));
         }


  //Reads track ids and names for showing the path
    /*-------------------------------------------------------------------*/
    QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
    n = db.exec(t);

    for(;n.next() == 1;) //If it is 1 it contains data
    {
        QString ss4 = n.value(1).toString();
        vertex_names.push_back(ss4.toStdString());
    }
    /*-------------------------------------------------------------------*/

    //Reads data for each track and how its connected along with its weight
    QString t2 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    qq = db.exec(t2);

    /*-------------------------------------------------------------------*/
    for(;qq.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = qq.value(0).toInt();
    int ss2 = qq.value(1).toInt();
    int ss3 = qq.value(2).toInt();
    adjacency_map[ss1].push_back(edge(ss2,  ss3));
    //qDebug() <<ss1<<",{"<<ss2<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/
    qDebug() <<"BFC"<< CHECKER;
    dij_main();//Call algorithm function
    qDebug() <<"AFC"<< CHECKER;
    }

    if (CHECKER == 1)//Use for checking purposes
    {
    adjacency_map.clear(); //Clears ajacency_map must be done to allow opposite direction of travel

    //Add Switch Positions closedPOS -> switch
    QString tqtps1 = QString("SELECT switch,position,straight,openPOS,closedPOS FROM switchInfoTable");
    p = db.exec(tqtps1);
    for(;p.next() == 1;) //If it is 1 it contains data
    {
        int ss1 = p.value(0).toInt();
        //int ss2 = p.value(1).toInt();
        //int ss3 = p.value(2).toInt();
        //int ss4 = p.value(3).toInt();
        int ss5 = p.value(4).toInt();
        adjacency_map[ss1].push_back(edge(ss5,  5));
    }

    //Reads track ids and names for showing the path
      /*-------------------------------------------------------------------*/
      QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
      n = db.exec(t);

      for(;n.next() == 1;) //If it is 1 it contains data
      {
          QString ss4 = n.value(1).toString();
          vertex_names.push_back(ss4.toStdString());
      }
      /*-------------------------------------------------------------------*/


    //Reads data for each track and how its connected along with its weight
    QString t3 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    qq = db.exec(t3);

    //For each switch in trackInfoTable execute the following:
    /*-------------------------------------------------------------------*/
    for(;qq.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = qq.value(0).toInt();
    int ss2 = qq.value(1).toInt();
    int ss3 = qq.value(2).toInt();
    adjacency_map[ss2].push_back(edge(ss1,  ss3));
    //qDebug() <<ss2<<",{"<<ss1<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/
    qDebug() <<"BFC"<< CHECKER;
    dij_main();//Call algorithm function
    qDebug() <<"AFC"<< CHECKER;
    }

    //*-------------------------------------------------------------------------------------------------------------------------------------------------
    //*-------------------------------------------------------------------------------------------------------------------------------------------------
    //*-------------------------------------------------------------------------------------------------------------------------------------------------

    //Start of Opening Switches
    if(CHECKER == 1)//Use for checking purposes
    {
        qDebug()<<"Initilize 3rd Part of Algorithm";
        adjacency_map.clear(); //Clears ajacency_map
        CHECKER = 0;

         //Add Switch Positions switch -> closedPOS
         QString tqtps1 = QString("SELECT switch,position,straight,openPOS,closedPOS FROM switchInfoTable");
         p = db.exec(tqtps1);

         for(;p.next() == 1;) //If it is 1 it contains data
         {
             int ss1 = p.value(0).toInt();
             //int ss2 = p.value(1).toInt();
             //int ss3 = p.value(2).toInt();
             int ss4 = p.value(3).toInt();
             //int ss5 = p.value(4).toInt();
             adjacency_map[ss4].push_back(edge(ss1,  5));
         }


  //Reads track ids and names for showing the path
    /*-------------------------------------------------------------------*/
    QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
    n = db.exec(t);

    for(;n.next() == 1;) //If it is 1 it contains data
    {
        QString ss4 = n.value(1).toString();
        vertex_names.push_back(ss4.toStdString());
    }
    /*-------------------------------------------------------------------*/

    //Reads data for each track and how its connected along with its weight
    QString t2 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    qq = db.exec(t2);

    /*-------------------------------------------------------------------*/
    for(;qq.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = qq.value(0).toInt();
    int ss2 = qq.value(1).toInt();
    int ss3 = qq.value(2).toInt();
    adjacency_map[ss1].push_back(edge(ss2,  ss3));
    //qDebug() <<ss1<<",{"<<ss2<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/
    qDebug() <<"BFC"<< CHECKER;
    dij_main();//Call algorithm function
    qDebug() <<"AFC"<< CHECKER;

    if (CHECKER == 1)//Use for checking purposes
    {
    adjacency_map.clear(); //Clears ajacency_map must be done to allow opposite direction of travel

    //Add Switch Positions closedPOS -> switch
    QString tqtps1 = QString("SELECT switch,position,straight,openPOS,closedPOS FROM switchInfoTable");
    p = db.exec(tqtps1);
    for(;p.next() == 1;) //If it is 1 it contains data
    {
        int ss1 = p.value(0).toInt();
        //int ss2 = p.value(1).toInt();
        //int ss3 = p.value(2).toInt();
        int ss4 = p.value(3).toInt();
        //int ss5 = p.value(4).toInt();
        adjacency_map[ss4].push_back(edge(ss1,  5));
    }

    //Reads track ids and names for showing the path
      /*-------------------------------------------------------------------*/
      QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
      n = db.exec(t);

      for(;n.next() == 1;) //If it is 1 it contains data
      {
          QString ss4 = n.value(1).toString();
          vertex_names.push_back(ss4.toStdString());
      }
      /*-------------------------------------------------------------------*/


    //Reads data for each track and how its connected along with its weight
    QString t3 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    qq = db.exec(t3);

    //For each switch in trackInfoTable execute the following:
    /*-------------------------------------------------------------------*/
    for(;qq.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = qq.value(0).toInt();
    int ss2 = qq.value(1).toInt();
    int ss3 = qq.value(2).toInt();
    adjacency_map[ss2].push_back(edge(ss1,  ss3));
    //qDebug() <<ss2<<",{"<<ss1<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/
    qDebug() <<"BFC"<< CHECKER;
    dij_main();//Call algorithm function
    qDebug() <<"AFC"<< CHECKER;
    }

    //*-------------------------------------------------------------------------------------------------------------------------------------------------
    //*-------------------------------------------------------------------------------------------------------------------------------------------------
    //*-------------------------------------------------------------------------------------------------------------------------------------------------

    //Start of Opening Switches
    if(CHECKER == 1)//Use for checking purposes
    {
        qDebug()<<"Initilize 4th Part of Algorithm";
        adjacency_map.clear(); //Clears ajacency_map
        CHECKER = 0;

         //Add Switch Positions switch -> closedPOS
         QString tqtps1 = QString("SELECT switch,position,straight,openPOS,closedPOS FROM switchInfoTable");
         p = db.exec(tqtps1);

         for(;p.next() == 1;) //If it is 1 it contains data
         {
             int ss1 = p.value(0).toInt();
             //int ss2 = p.value(1).toInt();
             //int ss3 = p.value(2).toInt();
             //int ss4 = p.value(3).toInt();
             int ss5 = p.value(4).toInt();
             adjacency_map[ss5].push_back(edge(ss1,  5));
         }


  //Reads track ids and names for showing the path
    /*-------------------------------------------------------------------*/
    QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
    n = db.exec(t);

    for(;n.next() == 1;) //If it is 1 it contains data
    {
        QString ss4 = n.value(1).toString();
        vertex_names.push_back(ss4.toStdString());
    }
    /*-------------------------------------------------------------------*/

    //Reads data for each track and how its connected along with its weight
    QString t2 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    qq = db.exec(t2);

    /*-------------------------------------------------------------------*/
    for(;qq.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = qq.value(0).toInt();
    int ss2 = qq.value(1).toInt();
    int ss3 = qq.value(2).toInt();
    adjacency_map[ss1].push_back(edge(ss2,  ss3));
    //qDebug() <<ss1<<",{"<<ss2<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/
    qDebug() <<"BFC"<< CHECKER;
    dij_main();//Call algorithm function
    qDebug() <<"AFC"<< CHECKER;

    if (CHECKER == 1)//Use for checking purposes
    {
    adjacency_map.clear(); //Clears ajacency_map must be done to allow opposite direction of travel

    //Add Switch Positions closedPOS -> switch
    QString tqtps1 = QString("SELECT switch,position,straight,openPOS,closedPOS FROM switchInfoTable");
    p = db.exec(tqtps1);
    for(;p.next() == 1;) //If it is 1 it contains data
    {
        int ss1 = p.value(0).toInt();
        //int ss2 = p.value(1).toInt();
        //int ss3 = p.value(2).toInt();
        //int ss4 = p.value(3).toInt();
        int ss5 = p.value(4).toInt();
        adjacency_map[ss5].push_back(edge(ss1,  5));
    }

    //Reads track ids and names for showing the path
      /*-------------------------------------------------------------------*/
      QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
      n = db.exec(t);

      for(;n.next() == 1;) //If it is 1 it contains data
      {
          QString ss4 = n.value(1).toString();
          vertex_names.push_back(ss4.toStdString());
      }
      /*-------------------------------------------------------------------*/


    //Reads data for each track and how its connected along with its weight
    QString t3 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    qq = db.exec(t3);

    //For each switch in trackInfoTable execute the following:
    /*-------------------------------------------------------------------*/
    for(;qq.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = qq.value(0).toInt();
    int ss2 = qq.value(1).toInt();
    int ss3 = qq.value(2).toInt();
    adjacency_map[ss2].push_back(edge(ss1,  ss3));
    //qDebug() <<ss2<<",{"<<ss1<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/
    qDebug() <<"BFC"<< CHECKER;
    dij_main();//Call algorithm function
    qDebug() <<"AFC"<< CHECKER;

        if (CHECKER == 1)
        {
            s = QString("Distance from vertex %1 to %2 is: %3").arg(start).arg(dest).arg("NULL-No Path Exists!");
            qDebug() << s;
            std::cout << "No Path Exists or switch is not correctly switched!"<< std::endl;
        }
        count = 0;
        CHECKER = 0;
    }
    }
    //qDebug() << "End of dij_mainprogram_copy";
}
}

/*-------------------------------------------------------------------------------------------------------------*/
//dij_main is called in dij_mainprogram to find the path
/*-------------------------------------------------------------------------------------------------------------*/

void MainWindow::dij_main()
{
    qDebug() << "Called dij_main()";
    count = 0;//count is used to output only one line of paths/distance

    QString ts1 = QString("SELECT ID,START,Direction,Destination from %1 WHERE ID=%2").arg("Trains").arg(ui->trainBox->currentText().toInt());
    TRAIN = db.exec(ts1);

    /*-------------------------------------------------------------------*/
    for(;TRAIN.next() == 1;) //If it is 1 it contains data
    {
    START = TRAIN.value(1).toString();
    DEST = TRAIN.value(3).toString();
    }

    if (DEST == "EMPTY")
    {
        ERROR = 1;
        QMessageBox msgBox;
        msgBox.setWindowTitle("ERROR: No Destination Found!");
        msgBox.setText("No Destination Found, Schedule a destination!");
        QSpacerItem* horizontalSpacer = new QSpacerItem(200, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
        msgBox.exec();
    }
    else if(START == DEST)
    {
        ERROR = 1;
        QMessageBox msgBox;
        msgBox.setWindowTitle("ERROR: At Destination!");
        msgBox.setText("At Destination, Schedule a different destination!");
        QSpacerItem* horizontalSpacer = new QSpacerItem(200, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
        msgBox.exec();
    }
    else
    {
    ERROR = 0;
    QString t3 = QString("SELECT trackID,trackNAME from %1").arg("tracklistingTable");
    n = db.exec(t3);

    //Pull source and destination name using trackid
    /*-------------------------------------------------------------------*/
    for(;n.next() == 1;) //If it is 1 it contains data
    {
    int stqs1 = n.value(0).toInt();
    QString stqs2 = n.value(1).toString();
    //qDebug() << stqs1 << stqs2;

    if (START == stqs2)
    {
        start = stqs1;
        qDebug() <<"Start:                 "<<START;
    }
    //qDebug() << START << start;
    if (DEST == stqs2)
    {
        dest = stqs1;
        qDebug() <<"Destination:               "<<DEST;
    }
    //qDebug() << DEST << dest;

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
        //MIN_DISTANCE = min_distance[v];

        if (v == dest)
        {
           MIN_DISTANCE = min_distance[v];
           qDebug() <<"Minimum Distance of V: "<< MIN_DISTANCE;
           if(count == 0)
           {
            if (min_distance[v] == inf)
            {
            qDebug() <<"inf distance";
            CHECKER = 1;
            }
            else
            {
            CHECKER = 0;
            s = QString("Distance from vertex %1 to %2 is: %3").arg(START).arg(DEST).arg(min_distance[v]);
            qDebug() << s;
            std::list<vertex_t> path =
                DijkstraGetShortestPathTo(v, previous);
            std::list<vertex_t>::iterator path_iter = path.begin();
            s2 = QString("Path: ");
            for( ; path_iter != path.end(); path_iter++)
            {
                QString s3;
                if(*path_iter == dest)
                {
                    s3 = QString("%1").arg(QString::fromStdString(vertex_names[*path_iter]));
                }
                else
                {
                s3 = QString("%1").arg(QString::fromStdString(vertex_names[*path_iter]));
                }
                PATH.append(s3);
            }
            }
            }
           count++;//Increments count by 1 to prevent more than one line of outputs
        }
    }
  }
}
