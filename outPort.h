//
// Created by Dvir Bartov on 18/03/2024.
//

#ifndef HW4_OUTPORT_H
#define HW4_OUTPORT_H

#include <deque>

using namespace std;

class outPort {
    double prob;
    int outPortID;
    deque<double> queue;
    double currServiceTimeLeft;
    int maxSize;
    int deliveredPackets;
    int droppedPackets;
    double totalWaitTime;
    double totalServiceTime;

public:
    outPort(int id, double prob);
    void insertPacket(double arrivalTime);
    double deliverPackets(double interval)
};


#endif //HW4_OUTPORT_H
