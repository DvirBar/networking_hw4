//
// Created by Dvir Bartov on 18/03/2024.
//
#include "inPort.h"
#include <random>

inPort::inPort(double prob, int index, double maxArrivalTime):
    prob(prob),
    index(index),
    maxArrivalTime(maxArrivalTime)
{}

vector<pair<int, double>> inPort::simulateArrivals() {
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<> exp(prob);
    double accumulatedTime = exp(gen);
    vector<pair<int, double>> portArrivalQueue;

    while(accumulatedTime <= maxArrivalTime) {
        pair<int, double> frame(index, accumulatedTime);
        portArrivalQueue.push_back(frame);
        accumulatedTime += exp(gen);
    }

    return portArrivalQueue;
}