#define MAX_VER 1000 // Maximum number of vertices
#define INFINITE 0x3fffffff // 7*f ~ 1.000.000.000

#include <vector>
#include <queue>
#include <iostream>

using namespace std;

struct edge {
    int to;
    int length;
    edge(int to, int length) : to(to), length(length) {}
};

struct vertex {
    vector<edge> edges;
    int dis;
    int prev;
};

vertex vertices[MAX_VER];

void reset() {
    for (int i=0; i < MAX_VER; i++) {
        vertices[i].edges.clear();
        vertices[i].dis = INFINITE;
        vertices[i].prev = -1;
    }
}

void addedge(int from, int to, int length=-1, bool directed=true) {
    vertices[from].edges.push_back(edge(to, length));
    if (!directed) vertices[to].edges.push_back(edge(from, length));
}

typedef pair<int, int> pp;
void dijkstra(int source) {
    //distance, vertex
    priority_queue<pp, vector<pp>, greater<pp> > q;
    vertices[source].dis = 0;
    q.push(make_pair(0, source));
    while (!q.empty()) {
        int u = q.top().second;
        int dis = q.top().first;
        q.pop();
        if (dis > vertices[u].dis) continue;
        for (size_t i = 0; i < vertices[u].edges.size(); i++) {
            edge &e = vertices[u].edges[i];
            if (dis + e.length < vertices[e.to].dis) {
                vertices[e.to].dis = dis + e.length;
                vertices[e.to].prev = u;
                q.push(make_pair(vertices[e.to].dis, e.to));
            }
        }
    }
}

int main()
{
    int source = 3;
    int dest = 4;
    reset();
    addedge(0, 1, 5, false);
    addedge(0, 2, 9, false);
    addedge(0, 3, 4, false);
    addedge(0, 4, 6, false);
    addedge(1, 2, 2, false);
    addedge(1, 3, 5, false);
    addedge(1, 4, 7, false);
    addedge(2, 3, 1, false);
    addedge(2, 4, 8, false);
    addedge(3, 4, 3, false);

    dijkstra(source);
    cout << "Distance from vertex "<< source <<" to "<< dest <<" is: " << vertices[dest].dis << endl;
    return 0;
}
