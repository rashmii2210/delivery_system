#include "dijkstra.h"
#include <iostream>
#include <algorithm>

Dijkstra::Dijkstra(Graph& g) : graph(g) {}

vector<float> Dijkstra::shortestPath(int src) {
    int n = graph.numNodes;
    vector<float> dist(n, INT_MAX);
    vector<int> prev(n, -1);
    priority_queue<pfi, vector<pfi>, greater<pfi>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        float d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto edge : graph.adjList[u]) {
            int v = edge.to;
            float w = edge.dist;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

void Dijkstra::printPath(int src, int dest) {
    vector<float> dist = shortestPath(src);

    int n = graph.numNodes;
    vector<float> d(n, INT_MAX);
    vector<int> prev(n, -1);
    priority_queue<pfi, vector<pfi>, greater<pfi>> pq;

    d[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        float dd = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (dd > d[u]) continue;
        for (auto edge : graph.adjList[u]) {
            int v = edge.to;
            float w = edge.dist;
            if (d[u] + w < d[v]) {
                d[v] = d[u] + w;
                prev[v] = u;
                pq.push({d[v], v});
            }
        }
    }

    // Build path by tracing back from destination node
    vector<int> path;
    for (int at = dest; at != -1; at = prev[at])
        path.push_back(at);
    reverse(path.begin(), path.end());

    cout << "\n====== SHORTEST PATH ======\n";
    cout << "FROM : " << graph.locations[src].name  << "\n";
    cout << "TO   : " << graph.locations[dest].name << "\n";
    cout << "PATH : ";
    for (int i = 0; i < path.size(); i++) {
        cout << graph.locations[path[i]].name;
        if (i != path.size() - 1) cout << " --> ";
    }
    cout << "\nDIST : " << d[dest] << " km\n";
    cout << "===========================\n";
}