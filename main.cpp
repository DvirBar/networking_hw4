#include <iostream>
#include <random>
#include <vector>
#include <utility>
#include <deque>
#include "inPort.h"

using namespace std;

deque<pair<int, double>> mergeArrivals(vector<vector<pair<int, double>>> arrivalQueues) {
    deque<pair<int, double>> arrivals;
    vector<int> indexes(arrivalQueues.size(), 0);
    int numFinished = 0, minIndex = -1;
    double minValue = INT_MAX;

    while(numFinished < arrivalQueues.size()) {
        for(int i=0; i<arrivalQueues.size(); i++) {
            if(indexes[i] == arrivalQueues[i].size()) {
                continue;
            }

            pair<int, double> currentFrame = arrivalQueues[i][indexes[i]];
            if(currentFrame.second < minValue) {
                minValue = currentFrame.second;
                minIndex = currentFrame.first;
            }
        }

        arrivals.emplace_back(minIndex, minValue);
        indexes[minIndex]++;

        if(indexes[minIndex] == arrivalQueues[minIndex].size()) {
            numFinished++;
        }

        minValue = INT_MAX;
        minIndex = -1;
    }

    return arrivals;
}

int simulateOutPort(int* probInOutMatrix, int inPort, int numOutPorts) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0,1.0);
    double rand = dis(gen);
    double cumulativeProb = 0.0;
    for(int i = 0; i < numOutPorts; i++) {
        cumulativeProb += probInOutMatrix[numOutPorts * inPort + i];
        if(rand < cumulativeProb) {
            return i;
        }
    }
    return -1; //shouldn't get here
}

void processArgs(char *argv[], int* maxArrivalTime, int* numInPorts, int* numOutPorts,
                 int* probInOutMatrix, double* inProb, int* outQueueSizes, double* outProbs) {
    int currentArgIndex = 1;
    *maxArrivalTime = atoi(argv[currentArgIndex]);
    currentArgIndex++;
    *numInPorts = atoi(argv[currentArgIndex]);
    currentArgIndex++;
    *numOutPorts = atoi(argv[currentArgIndex]);
    currentArgIndex++;
    probInOutMatrix = new int[(*numInPorts)*(*numOutPorts)];

    for(int i=0; i<*numInPorts; i++) {
        for(int j=0; j<*numOutPorts; j++) {
            probInOutMatrix[i*(*numOutPorts)+j] = atoi(argv[currentArgIndex]);
            currentArgIndex++;
        }
    }

    inProb = new double[*numInPorts];

    for(int i=0; i<*numInPorts; i++) {
        inProb[i] = atof(argv[currentArgIndex]);
        currentArgIndex++;
    }

    outQueueSizes = new int[*numOutPorts];

    for(int i=0; i<*numOutPorts; i++) {
        outQueueSizes[i] = atoi(argv[currentArgIndex]);
        currentArgIndex++;
    }

    outProbs = new double[*numOutPorts];

    for(int i=0; i<*numOutPorts; i++) {
        outProbs[i] = atof(argv[currentArgIndex]);
        currentArgIndex++;
    }
}

int main(int argc, char *argv[]) {
    int maxArrivalTime, numInPorts, numOutPorts;
    int* probInOutMatrix = nullptr;
    double* inProb = nullptr;
    int* outQueueSizes = nullptr;
    double* outProbs = nullptr;

    processArgs(argv, &maxArrivalTime, &numInPorts, &numOutPorts,
                probInOutMatrix, inProb, outQueueSizes, outProbs);

    vector<vector<pair<int, double>>> arrivalQueues;

    // TODO: fix the nullptr thing
    for(int i=0; i<numInPorts; i++) {
        inPort port = inPort(inProb[i], i, maxArrivalTime);
        arrivalQueues.push_back(port.simulateArrivals());
    }

    deque<pair<int, double>> arrivals = mergeArrivals(arrivalQueues);


    delete[] probInOutMatrix;
    delete[] inProb;
    delete[] outQueueSizes;
    delete[] outProbs;

    return 0;
}
