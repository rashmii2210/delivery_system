#ifndef ORDER_H
#define ORDER_H
#include <string>
using namespace std;

struct Order {
    int id;
    int pickupNode;   // restaurant node
    int dropoffNode;  // customer node
    int priority;     // 1 = urgent, 2 = normal
    bool assigned;

    Order(int id, int pickup, int dropoff, int priority)
        : id(id), pickupNode(pickup), dropoffNode(dropoff),
          priority(priority), assigned(false) {}
};

#endif