#include "graph.h"

Graph::Graph(int n) {
    numNodes = n;
    adjList.resize(n);
    locations.resize(n);
}

void Graph::addLocation(int id, string name, LocationType type) {
    locations[id] = {id, name, type};
}

void Graph::addEdge(int from, int to, float dist) {
    // Undirected graph
    adjList[from].push_back({to, dist});
    adjList[to].push_back({from, dist});
}

void Graph::printGraph() {
    cout << "\n====== CITY MAP (GRAPH) ======\n";
    for (int i = 0; i < numNodes; i++) {
        cout << "[" << locations[i].name << "] connects to:\n";
        for (auto edge : adjList[i]) {
            cout << "   --> " << locations[edge.to].name
                 << " (" << edge.dist << " km)\n";
        }
    }
    cout << "==============================\n";
}