#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// pair<distance, nodeId> — priority queue uses this order for sorting
typedef pair<float, int> pfi;

class Dijkstra {
public:
    Graph& graph;

    Dijkstra(Graph& g);

    vector<float> shortestPath(int src);

    void printPath(int src, int dest);
};

#endif