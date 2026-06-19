#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "graph.h"
#include "dijkstra.h"
#include "rider.h"
#include "order.h"
#include <vector>
#include <unordered_map>
using namespace std;

class AssignmentEngine {
public:
    Graph& graph;
    Dijkstra dijk;
    vector<Rider>& riders;
    vector<Order>& orders;
    bool verbose;

    // CACHE: maps "rider's current node" -> "distances to all other nodes"
    // This avoids recalculating Dijkstra when multiple riders share the
    // same location, or when we already computed it before
    unordered_map<int, vector<float>> distanceCache;

    AssignmentEngine(Graph& g, vector<Rider>& r, vector<Order>& o, bool v = true)
        : graph(g), dijk(g), riders(r), orders(o), verbose(/*verbose=*/v) {}

    void assignAll();
    void printSummary();
    void exportJSON();

    vector<float>& getDistancesFrom(int node);
};

#endif