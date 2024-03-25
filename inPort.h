//
// Created by Dvir Bartov on 18/03/2024.
//

#ifndef HW4_INPORT_H
#define HW4_INPORT_H

#include <vector>
#include <utility>

using namespace std;

class inPort {
    double prob;
    int index;
    double maxArrivalTime;

public:
    inPort(double prob, int index, double maxArrivalTime);
    vector<pair<int, double>> simulateArrivals();
};


#endif //HW4_INPORT_H
