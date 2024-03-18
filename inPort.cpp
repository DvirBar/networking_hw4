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

vector<pair<int, double>> &inPort::simulateArrivals() {
    exponential_distribution<> exp(1/prob);


}