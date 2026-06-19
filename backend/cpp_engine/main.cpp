#include "graph.h"
#include "dijkstra.h"
#include "order.h"
#include "rider.h"
#include "assignment.h"
#include "io_handler.h"
#include "generator.h"
#include <chrono>
#include <iostream>
using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[])
{
    // Usage:
    //   ./delivery input.json output.json                   -> normal file mode
    //   ./delivery benchmark 100 500 50 1000 output.json    -> benchmark mode

    string mode = (argc > 1) ? argv[1] : "file";

    if (mode == "benchmark")
    {
        int numNodes = (argc > 2) ? stoi(argv[2]) : 100;
        int numEdges = (argc > 3) ? stoi(argv[3]) : 500;
        int numRiders = (argc > 4) ? stoi(argv[4]) : 50;
        int numOrders = (argc > 5) ? stoi(argv[5]) : 1000;
        string outputFile = (argc > 6) ? argv[6] : "output.json";

        cout << "Generating random city: " << numNodes << " nodes, "
             << numEdges << " edges, " << numRiders << " riders, "
             << numOrders << " orders...\n";

        Graph city = Generator::generateGraph(numNodes, numEdges);
        vector<Rider> riders = Generator::generateRiders(numRiders, numNodes);
        vector<Order> orders = Generator::generateOrders(numOrders, numNodes);

        auto start = high_resolution_clock::now();

        AssignmentEngine engine(city, riders, orders, /*verbose=*/false);
        engine.assignAll();

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);

        cout << "Benchmark completed in " << duration.count() << " ms\n";

        IOHandler::writeOutput(outputFile, city, riders, orders, duration.count());
        cout << "Results written to " << outputFile << "\n";
    }
    else
    {
        string inputFile = (argc > 1) ? argv[1] : "input.json";
        string outputFile = (argc > 2) ? argv[2] : "output.json";

        Graph *city = nullptr;
        vector<Rider> riders;
        vector<Order> orders;

        IOHandler::loadInput(inputFile, city, riders, orders);

        cout << "Loaded " << city->numNodes << " nodes, "
             << riders.size() << " riders, "
             << orders.size() << " orders.\n";

        auto start = high_resolution_clock::now();

        AssignmentEngine engine(*city, riders, orders, /*verbose=*/false);
        engine.assignAll();

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);

        cout << "Assignment completed in " << duration.count() << " ms\n";

        IOHandler::writeOutput(outputFile, *city, riders, orders, duration.count());
        cout << "Results written to " << outputFile << "\n";

        delete city;
    }

    return 0;
}