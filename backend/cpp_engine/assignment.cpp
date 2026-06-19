#include "assignment.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <climits>
using namespace std;

vector<float>& AssignmentEngine::getDistancesFrom(int node) {
    if (distanceCache.find(node) == distanceCache.end()) {
        distanceCache[node] = dijk.shortestPath(node);
    }
    return distanceCache[node];
}

void AssignmentEngine::assignAll() {
    if (verbose) cout << "\n====== ASSIGNMENT ENGINE ======\n";

    for (auto& order : orders) {
        float bestDist = INT_MAX;
        int bestRider = -1;

        if (verbose) {
            cout << "\n+--------------------------------------------------+\n";
            cout << "| ORDER #" << order.id
                 << " | " << graph.locations[order.pickupNode].name
                 << " --> " << graph.locations[order.dropoffNode].name
                 << " | Priority: " << (order.priority == 1 ? "URGENT" : "NORMAL")
                 << "\n";
            cout << "+--------------------------------------------------+\n";
            cout << left
                 << setw(10) << "| Rider"
                 << setw(12) << "| Dist"
                 << setw(12) << "| Time"
                 << setw(12) << "| Workload"
                 << setw(11) << "| Score"
                 << "|\n";
            cout << "+----------+-----------+-----------+-----------+---------+\n";
        }

        for (int i = 0; i < riders.size(); i++) {
            vector<float>& dist = getDistancesFrom(riders[i].currentNode);
            float toPickup = dist[order.pickupNode];

            float timeToPickup = toPickup / 20.0 * 60;

            float score = toPickup + (riders[i].workload * 2.0)
                        - (order.priority == 1 ? 3.0 : 0)
                        + (timeToPickup * 0.1);

            if (verbose) {
                cout << left
                     << "| " << setw(8)  << riders[i].name
                     << "| " << setw(10) << (to_string((int)toPickup) + " km")
                     << "| " << setw(10) << (to_string((int)timeToPickup) + " mins")
                     << "| " << setw(10) << riders[i].workload
                     << "| " << setw(9)  << score
                     << "|\n";
            }

            if (score < bestDist) {
                bestDist = score;
                bestRider = i;
            }
        }

        if (verbose) {
            cout << "+----------+-----------+-----------+-----------+---------+\n";
            cout << "   >>> ASSIGNED TO: Rider " << riders[bestRider].name
                 << " (best score: " << bestDist << ")\n";
        }

        riders[bestRider].assignedOrders.push_back(order.id);
        riders[bestRider].workload++;
        
        // Rider's location changes -> their OLD cached distances become stale
        // but we don't delete them (other riders might still be at that node)
        // We just compute fresh distances next time THIS rider is checked
        riders[bestRider].currentNode = order.dropoffNode;
        order.assigned = true;
    }
}

void AssignmentEngine::printSummary() {
    cout << "\n====== FINAL SUMMARY ======\n";
    cout << "+--------------------------------------------------+\n";

    for (int i = 0; i < riders.size(); i++) {
        cout << "Rider: " << riders[i].name
             << " | Orders: " << riders[i].workload
             << " | Location: " << graph.locations[riders[i].currentNode].name << "\n";
        cout << "+--------------------------------------------------+\n";

        for (int j = 0; j < riders[i].assignedOrders.size(); j++) {
            int oid = riders[i].assignedOrders[j];
            cout << "   Order #" << oid
                 << " | " << graph.locations[orders[oid].pickupNode].name
                 << " --> " << graph.locations[orders[oid].dropoffNode].name << "\n";
        }
        cout << "+--------------------------------------------------+\n";
    }
    cout << "===========================\n";
}

void AssignmentEngine::exportJSON() {
    ofstream file("output.json");
    file << "{\n";
    file << "  \"riders\": [\n";
    for (int i = 0; i < riders.size(); i++) {
        file << "    {\n";
        file << "      \"name\": \"" << riders[i].name << "\",\n";
        file << "      \"orders\": " << riders[i].workload << ",\n";
        file << "      \"location\": \"" << graph.locations[riders[i].currentNode].name << "\",\n";
        file << "      \"assignedOrders\": [";
        for (int j = 0; j < riders[i].assignedOrders.size(); j++) {
            int oid = riders[i].assignedOrders[j];
            file << "\"#" << oid << " "
                 << graph.locations[orders[oid].pickupNode].name
                 << " to "
                 << graph.locations[orders[oid].dropoffNode].name << "\"";
            if (j != riders[i].assignedOrders.size() - 1) file << ", ";
        }
        file << "]\n";
        file << "    }";
        if (i != riders.size() - 1) file << ",";
        file << "\n";
    }
    file << "  ]\n";
    file << "}\n";
    file.close();
    cout << "\n[output.json written successfully]\n";
}