//
// Created by Dvir Bartov on 18/03/2024.
//

#include "outPort.h"
#include <random>

outPort::outPort(int id, double prob):
    outPortID(id),
    prob(prob),
    queue(),
    deliveredPackets(0),
    timeInService(0),
    droppedPackets(0),
    totalWaitTime(0),
    totalServiceTime(0)
{}

void outPort::insertPacket(double arrivalTime) {
    if(queue.size()-1 > maxSize) {
        droppedPackets++;
        return;
    }

    queue.push_back(arrivalTime);
    deliveredPackets++;
}

double outPort::deliverPackets(double intervalStart, double intervalEnd) {
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<> exp(1/prob);
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
            currServiceTimeLeft = -1;
            totalWaitTime += intervalStart + currIntervalTime - queue.front();
            totalServiceTime += currServiceTimeLeft + timeInService;
            queue.pop_front();
            timeInService = 0;
        } else {
            timeInService += interval - currIntervalTime;
            currServiceTimeLeft -= interval - currIntervalTime;
            currIntervalTime = interval;
        }
    }

    return lastServiceTimeLeft;
}

