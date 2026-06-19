#include "generator.h"
#include <iostream>
using namespace std;

Graph Generator::generateGraph(int numNodes, int numEdges) {
    Graph g(numNodes);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> nodeDist(0, numNodes - 1);
    uniform_real_distribution<float> distDist(0.5, 10.0);

    for (int i = 0; i < numNodes; i++) {
        LocationType type;
        if (i % 3 == 0) type = RESTAURANT;
        else if (i % 3 == 1) type = CUSTOMER;
        else type = HUB;

        g.addLocation(i, "Node" + to_string(i), type);
    }

    // Connect nodes in a line first (ensures graph is connected)
    for (int i = 0; i < numNodes - 1; i++) {
        float w = distDist(gen);
        g.addEdge(i, i + 1, w);
    }

    int edgesAdded = numNodes - 1;
    while (edgesAdded < numEdges) {
        int u = nodeDist(gen);
        int v = nodeDist(gen);
        if (u != v) {
            float w = distDist(gen);
            g.addEdge(u, v, w);
            edgesAdded++;
        }
    }

    return g;
}

vector<Rider> Generator::generateRiders(int numRiders, int numNodes) {
    vector<Rider> riders;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> nodeDist(0, numNodes - 1);

    for (int i = 0; i < numRiders; i++) {
        int startNode = nodeDist(gen);
        riders.push_back(Rider(i, "Rider" + to_string(i), startNode));
    }
    return riders;
}

vector<Order> Generator::generateOrders(int numOrders, int numNodes) {
    vector<Order> orders;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> nodeDist(0, numNodes - 1);
    uniform_int_distribution<int> priorityDist(1, 2);

    for (int i = 0; i < numOrders; i++) {
        int pickup = nodeDist(gen);
        int dropoff = nodeDist(gen);
        while (dropoff == pickup) dropoff = nodeDist(gen);

        int priority = priorityDist(gen);
        orders.push_back(Order(i, pickup, dropoff, priority));
    }
    return orders;
}