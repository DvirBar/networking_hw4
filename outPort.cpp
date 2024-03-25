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

double outPort::deliverPackets(double interval) {
    if(currServiceTimeLeft == -1) {
        random_device rd;
        mt19937 gen(rd());
        exponential_distribution<> exp(1/prob);
        currServiceTimeLeft = exp(gen);
    }
}

