#include "io_handler.h"
#include <iostream>

void IOHandler::loadInput(const string& filename,
                           Graph*& graph,
                           vector<Rider>& riders,
                           vector<Order>& orders) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: Could not open " << filename << "\n";
        exit(1);
    }

    json data;
    file >> data;

    int numNodes = data["nodes"].size();
    graph = new Graph(numNodes);

    for (auto& node : data["nodes"]) {
        int id = node["id"];
        string name = node["name"];
        string typeStr = node["type"];

        LocationType type;
        if (typeStr == "RESTAURANT") type = RESTAURANT;
        else if (typeStr == "CUSTOMER") type = CUSTOMER;
        else type = HUB;

        graph->addLocation(id, name, type);
    }

    for (auto& edge : data["edges"]) {
        int from = edge["from"];
        int to = edge["to"];
        float dist = edge["dist"];
        graph->addEdge(from, to, dist);
    }

    for (auto& r : data["riders"]) {
        int id = r["id"];
        string name = r["name"];
        int startNode = r["startNode"];
        riders.push_back(Rider(id, name, startNode));
    }

    for (auto& o : data["orders"]) {
        int id = o["id"];
        int pickup = o["pickup"];
        int dropoff = o["dropoff"];
        int priority = o["priority"];
        orders.push_back(Order(id, pickup, dropoff, priority));
    }
}

void IOHandler::writeOutput(const string& filename,
                             Graph& graph,
                             vector<Rider>& riders,
                             vector<Order>& orders,
                             long long timeTakenMs) {
    json output;
    output["timeTakenMs"] = timeTakenMs;
    output["totalOrders"] = orders.size();
    output["totalRiders"] = riders.size();

    json ridersJson = json::array();
    for (auto& r : riders) {
        json riderObj;
        riderObj["id"] = r.id;
        riderObj["name"] = r.name;
        riderObj["workload"] = r.workload;
        riderObj["currentLocation"] = graph.locations[r.currentNode].name;

        json assignedOrdersJson = json::array();
        for (int oid : r.assignedOrders) {
            json orderObj;
            orderObj["orderId"] = oid;
            orderObj["pickup"] = graph.locations[orders[oid].pickupNode].name;
            orderObj["dropoff"] = graph.locations[orders[oid].dropoffNode].name;
            orderObj["priority"] = orders[oid].priority;
            assignedOrdersJson.push_back(orderObj);
        }
        riderObj["assignedOrders"] = assignedOrdersJson;
        ridersJson.push_back(riderObj);
    }
    output["riders"] = ridersJson;

    ofstream file(filename);
    file << output.dump(2);
    file.close();
}