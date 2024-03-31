//
// Created by Dvir Bartov on 18/03/2024.
//

#include "outPort.h"
#include <random>

outPort::outPort(int id, double prob, int maxSize):
    prob(prob),
    outPortID(id),
    queue(),
    currServiceTimeLeft(-1),
    timeInService(0),
    maxSize(maxSize),
    deliveredPackets(0),
    droppedPackets(0),
    totalWaitTime(0),
    totalServiceTime(0)
{}

void outPort::insertPacket(double arrivalTime) {
    if(queue.size() != 0 && queue.size()-1 > maxSize) {
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

    while(currIntervalTime < interval && !queue.empty()) {
        if(currServiceTimeLeft == -1) {
            currServiceTimeLeft = exp(gen);
        }

        if(currServiceTimeLeft <= interval - currIntervalTime) {
            currIntervalTime += currServiceTimeLeft;
            lastServiceTimeLeft = currServiceTimeLeft;
            totalWaitTime += intervalStart + currIntervalTime - queue.front();
            totalServiceTime += currServiceTimeLeft + timeInService;
            queue.pop_front();
            currServiceTimeLeft = -1;
            timeInService = 0;
        } else {
            timeInService += interval - currIntervalTime;
            currServiceTimeLeft -= timeInService;
            currIntervalTime = interval;
        }
    }

    return lastServiceTimeLeft;
}

