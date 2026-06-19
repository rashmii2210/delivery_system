#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum LocationType {
    RESTAURANT,
    CUSTOMER,
    HUB  // Where riders start
};

struct Location {
    int id;
    string name;
    LocationType type;
};

struct Edge {
    int to;
    float dist;
};

class Graph {
public:
    int numNodes;
    vector<Location> locations;
    vector<vector<Edge>> adjList;

    Graph(int n);
    void addLocation(int id, string name, LocationType type);
    void addEdge(int from, int to, float dist);
    void printGraph();
};

#endif