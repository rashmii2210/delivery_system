#ifndef GENERATOR_H
#define GENERATOR_H

#include "graph.h"
#include "rider.h"
#include "order.h"
#include <vector>
#include <random>
using namespace std;

class Generator {
public:
    static Graph generateGraph(int numNodes, int numEdges);

    static vector<Rider> generateRiders(int numRiders, int numNodes);

    static vector<Order> generateOrders(int numOrders, int numNodes);
};

#endif