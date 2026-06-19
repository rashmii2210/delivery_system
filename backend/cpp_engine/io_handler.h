#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include "json.hpp"
#include "graph.h"
#include "rider.h"
#include "order.h"
#include <vector>
#include <fstream>
using namespace std;
using json = nlohmann::json;

class IOHandler {
public:
    static void loadInput(const string& filename,
                           Graph*& graph,
                           vector<Rider>& riders,
                           vector<Order>& orders);

    static void writeOutput(const string& filename,
                             Graph& graph,
                             vector<Rider>& riders,
                             vector<Order>& orders,
                             long long timeTakenMs);
};

#endif