/* The authors of this work have released all rights to it and placed it
in the public domain under the Creative Commons CC0 1.0 waiver
(http://creativecommons.org/publicdomain/zero/1.0/).

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Dijkstra's_algorithm_(C_Plus_Plus)?oldid=19645
*/

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <set>
#include <limits>
#include <QDebug>
#include "dialog.h"
#include "ui_dialog.h"

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

void Dialog::dij_mainprogram() //Main Program
{
     adjacency_map.clear(); //Clears ajacency_map
     CHECKER = 0;

    //Populate switches
    /*-------------------------------------------------------------------*/
        QString s = QString("SELECT Switch,Position from %3").arg("switchInfoTable");
        q = db.exec(s);

    //For each swich in switchInfoTable execute the following:
    for(;q.next() == 1;) //If it is 1 it contains data
    {
        //QString ss1 = q.value(0).toString();
        //QString ss2 = q.value(1).toChar();
        //qDebug() << "(" << ss1 << ", " << ss2 << ")";
        //qDebug() << "(" << ss1 << ")";
        //vertex_names.push_back(ss1.toStdString());
    }
    /*-------------------------------------------------------------------*/

    //Reads track ids and names for showing the path
    /*-------------------------------------------------------------------*/
    QString t = QString("SELECT trackID,trackNAME from %3").arg("tracklistingTable");
    q = db.exec(t);

    for(;q.next() == 1;) //If it is 1 it contains data
    {
        QString ss4 = q.value(1).toString();
        //qDebug() << "(" << ss1 <<","<< ss2 <<","<< ss3 << ")";
        vertex_names.push_back(ss4.toStdString());
    }
    /*-------------------------------------------------------------------*/

    //Reads data for each track and how its connected along with its weight
    QString t2 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    q = db.exec(t2);

    //For each swich in trackInfoTable execute the following:
    /*-------------------------------------------------------------------*/
    for(;q.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = q.value(0).toInt();
    int ss2 = q.value(1).toInt();
    int ss3 = q.value(2).toInt();
    adjacency_map[ss1].push_back(edge(ss2,  ss3));
    //qDebug() <<ss1<<",{"<<ss2<<","<<ss3<<"}";
    //adjacency_map[ss2].push_back(edge(ss1,  ss3));
    //qDebug() <<ss2<<",{"<<ss1<<","<<ss3<<"}";
    }
    /*-------------------------------------------------------------------*/

    dij_main();//Call algorithm function

    if (CHECKER == 1)//Use for checking purposes
    {
    adjacency_map.clear(); //Clears ajacency_map must be done to allow opposite direction of travel
    CHECKER = 0;
    //Reads data for each track and how its connected along with its weight
    QString t3 = QString("SELECT currentnode,nextnode,weight from %3").arg("trackInfoTable");
    q = db.exec(t3);

    //For each swich in trackInfoTable execute the following:
    /*-------------------------------------------------------------------*/
    for(;q.next() == 1;) //If it is 1 it contains data
    {
    int ss1 = q.value(0).toInt();
    int ss2 = q.value(1).toInt();
    int ss3 = q.value(2).toInt();
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
        ui->distanceEdit->setText(s);
        std::cout << "No Path Exists or switch is not correctly switched!"<< std::endl;
        s2 = QString("Path: No Path Exists or switch is not correctly switched!");
        ui->pathEdit->setText(s2);
    }
    }
}

//Dialog:: was added to iterface with Qt dialog.cpp
void Dialog::dij_main()
{
    count = 0;//count is used to output only one line of paths/distance

    //Range of spinboxes set in dialog.cpp
    //Starting position
    start = ui->sourceBox->value();

    //Destination
    dest = ui->destBox->value();

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
            s = QString("Distance from vertex %1 to %2 is: %3").arg(start).arg(dest).arg(min_distance[v]);
            ui->distanceEdit->clear();
            ui->distanceEdit->setText(s);
            std::cout << "Distance to " << vertex_names[v] << ": " << min_distance[v] << std::endl;
            std::list<vertex_t> path =
                DijkstraGetShortestPathTo(v, previous);
            std::list<vertex_t>::iterator path_iter = path.begin();
            s2 = QString("Path: ");
            ui->pathEdit->clear();
            ui->pathEdit->setText(s2);
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
                ui->pathEdit->setText(s2); 
            }
            std::cout << std::endl;
            }
            std::cout << std::endl;
            }
           count++;//Increments count by 1 to prevent more than one line of outputs
        }
    }
}
