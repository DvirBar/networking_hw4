//
// Created by Dvir Bartov on 18/03/2024.
//

#include "outPort.h"
#include <random>

outPort::outPort(int id, double prob, unsigned long maxSize):
    prob(prob),
    outPortID(id),
    queue(),
    currServiceTimeLeft(-1),
    timeInService(0),
    maxSize(maxSize),
    deliveredPackets(0),
    droppedPackets(0),
    totalWaitTime(0),
    servicedArrivalTime(0),
    totalServiceTime(0)
{}

void outPort::insertPacket(double arrivalTime) {
    if((int)queue.size() == maxSize) {
        droppedPackets++;
        return;
    }

    queue.push_back(arrivalTime);
    deliveredPackets++;
}

double outPort::deliverPackets(double intervalStart, double intervalEnd) {
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<> exp(prob);
    double currIntervalTime = 0;
    double interval = intervalEnd - intervalStart;
    double lastServiceTimeLeft = 0;

    while(currIntervalTime < interval && (!queue.empty() || currServiceTimeLeft != -1)) {
        if(currServiceTimeLeft == -1) {
            currServiceTimeLeft = exp(gen);
            servicedArrivalTime = queue.front();
            queue.pop_front();
        }

        if(currServiceTimeLeft <= interval - currIntervalTime) {
            currIntervalTime += currServiceTimeLeft;
            lastServiceTimeLeft = currServiceTimeLeft;
            totalWaitTime += intervalStart + currIntervalTime - servicedArrivalTime;
            totalServiceTime += currServiceTimeLeft + timeInService;
            currServiceTimeLeft = -1;
            timeInService = 0;
        } else {
            timeInService += interval - currIntervalTime;
            currServiceTimeLeft -= (interval - currIntervalTime);
            currIntervalTime = interval;
        }
    }

    return lastServiceTimeLeft;
}

