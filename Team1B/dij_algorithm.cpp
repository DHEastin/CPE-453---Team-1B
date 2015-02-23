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

//Dialog:: was added to iterface with Qt dialog.cpp
void Dialog::dij_mainprogram()
{
    int count = 0;//count is used to output only one line of paths/distance

    //Range of spinboxes set in dialog.cpp
    //Starting position
    start = ui->sourceBox->value();

    //Destination
    dest = ui->destBox->value();

    adjacency_map_t adjacency_map;
    std::vector<std::string> vertex_names;

    //0 = Straight 1 = Switched
    int SwitchStatus_switch1 = 0;
    int SwitchStatus_switch2 = 0;
    int SwitchStatus_switch3 = 0;
    int SwitchStatus_switch4 = 0;

    //Current path setup will be changing this to pull from SQL Server

    //Error when starting train Track 1-1. Can use for destination NOT source

    //Static labels to test algorithm

    vertex_names.push_back("Switch 1");      // 0
    vertex_names.push_back("Switch 2");      // 1
    vertex_names.push_back("Switch 3");      // 2
    vertex_names.push_back("Switch 4");      // 3
    vertex_names.push_back("Track 1");       // 4
    vertex_names.push_back("Track 2");       // 5
    vertex_names.push_back("Track 3");       // 6
    vertex_names.push_back("Track 4");       // 7
    vertex_names.push_back("Track 5");       // 8
    vertex_names.push_back("Track 6");       // 9
    vertex_names.push_back("Track 7");       // 10
    vertex_names.push_back("Track 8");       // 11
    vertex_names.push_back("Track 9");       // 12
    vertex_names.push_back("Track 10");      // 13
    vertex_names.push_back("Track 11");      // 14
    vertex_names.push_back("Track 12");      // 15
    vertex_names.push_back("Track 13");      // 16
    vertex_names.push_back("Track 14");      // 17


    //THE FOLLOWING MAPPING IS THE CENTER OF THE CPE453 TRACK SECTION
    //THE DISTANCES ARE88WRONG, I WILL CORRECT WHEN DATA IS AVAILABLE

    //Track Connections
    adjacency_map[5].push_back(edge(4,    5));
    adjacency_map[4].push_back(edge(5,    5));
    adjacency_map[5].push_back(edge(6,    5));
    adjacency_map[6].push_back(edge(5,    5));
    adjacency_map[8].push_back(edge(10,   5));
    adjacency_map[10].push_back(edge(8,   5));
    adjacency_map[10].push_back(edge(12,  5));
    adjacency_map[12].push_back(edge(10,  5));
    adjacency_map[9].push_back(edge(11,   5));
    adjacency_map[11].push_back(edge(9,   5));
    adjacency_map[11].push_back(edge(13,  5));
    adjacency_map[13].push_back(edge(11,  5));
    adjacency_map[16].push_back(edge(17,  5));
    adjacency_map[17].push_back(edge(16,  5));
    adjacency_map[15].push_back(edge(16,  5));
    adjacency_map[16].push_back(edge(15,  5));

    //Switch Connections
    //If switch is 0 Track is set for straight
    //If switch is 1 Track is switched
    if(SwitchStatus_switch1 == 0)
    {
        adjacency_map[6].push_back(edge(0,  5)); //Switch 14
        adjacency_map[0].push_back(edge(6,  5)); //Switch 14
    }
    else
    {
        adjacency_map[4].push_back(edge(0,  5)); //Switch 14
        adjacency_map[0].push_back(edge(4,  5)); //Switch 14
    }
    if(SwitchStatus_switch2 == 0)
    {
        adjacency_map[1].push_back(edge(8,  5)); //Switch 43
        adjacency_map[8].push_back(edge(1,  5)); //Switch 43
    }
    else
    {
        adjacency_map[1].push_back(edge(9,  5)); //Switch 43
        adjacency_map[9].push_back(edge(1,  5)); //Switch 43
    }
    if(SwitchStatus_switch3 == 0)
    {
        adjacency_map[2].push_back(edge(12, 5)); //Switch 52
        adjacency_map[12].push_back(edge(2, 5)); //Switch 52
    }
    else
    {
        adjacency_map[2].push_back(edge(13, 5)); //Switch 52
        adjacency_map[13].push_back(edge(2, 5)); //Switch 52
    }
    if(SwitchStatus_switch4 == 0)
    {
        adjacency_map[17].push_back(edge(3, 5)); //Switch 84
        adjacency_map[3].push_back(edge(17, 5)); //Switch 84
    }
    else
    {
        adjacency_map[15].push_back(edge(3, 5)); //Switch 84
        adjacency_map[3].push_back(edge(15, 5)); //Switch 84
    }

    //Stand true regardless of switch status (Back part of Switch)
    adjacency_map[0].push_back(edge(7,  15)); //Switch 1
    adjacency_map[7].push_back(edge(0,  15)); //Switch 1
    adjacency_map[1].push_back(edge(7,  15)); //Switch 2
    adjacency_map[7].push_back(edge(1,  15)); //Switch 2
    adjacency_map[2].push_back(edge(14, 15)); //Switch 3
    adjacency_map[14].push_back(edge(2, 15)); //Switch 3
    adjacency_map[3].push_back(edge(14, 15)); //Switch 4
    adjacency_map[14].push_back(edge(3, 15)); //Switch 4

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
            s = QString("Distance from vertex %1 to %2 is: %3").arg(start).arg(dest).arg("NULL-No Path Exists!");
            ui->distanceEdit->setText(s);
            std::cout << "No Path Exists or switch is not correctly switched!"<< std::endl;
            s2 = QString("Path: No Path Exists or switch is not correctly switched!");
            ui->pathEdit->setText(s2);
            }
            else
            {
            s = QString("Distance from vertex %1 to %2 is: %3").arg(start).arg(dest).arg(min_distance[v]);
            ui->distanceEdit->setText(s);
            std::cout << "Distance to " << vertex_names[v] << ": " << min_distance[v] << std::endl;
            std::list<vertex_t> path =
                DijkstraGetShortestPathTo(v, previous);
            std::list<vertex_t>::iterator path_iter = path.begin();
            s2 = QString("Path: ");
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
