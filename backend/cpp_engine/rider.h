#ifndef RIDER_H
#define RIDER_H
#include <vector>
#include <string>
using namespace std;

struct Rider {
    int id;
    string name;
    int currentNode;  // where rider is right now
    int workload;     // how many orders assigned
    vector<int> assignedOrders; // order ids

    Rider(int id, string name, int startNode)
        : id(id), name(name), currentNode(startNode), workload(0) {}
};

#endif