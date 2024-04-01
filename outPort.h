//
// Created by Dvir Bartov on 18/03/2024.
//

#ifndef HW4_OUTPORT_H
#define HW4_OUTPORT_H

#include <deque>

using namespace std;

class outPort {
public:
    double prob;
    int outPortID;
    deque<double> queue;
    double servicedArrivalTime;
    double currServiceTimeLeft;
    double timeInService;
    unsigned long maxSize;
    unsigned long deliveredPackets;
    unsigned long droppedPackets;
    double totalWaitTime;
    double totalServiceTime;

    outPort(int id, double prob, unsigned long maxSize);
    void insertPacket(double arrivalTime);
    double deliverPackets(double intervalStart, double intervalEnd);
};


#endif //HW4_OUTPORT_H
