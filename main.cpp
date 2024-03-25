#include <iostream>
#include <random>
#include <vector>
#include <utility>
#include <deque>
#include "inPort.h"
#include "outPort.h"

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

int simulateOutPort(double* probInOutMatrix, int inPort, int numOutPorts) {
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
                 double* probInOutMatrix, double* inProb, int* outQueueSizes, double* outProbs) {
    int currentArgIndex = 1;
    *maxArrivalTime = atoi(argv[currentArgIndex]);
    currentArgIndex++;
    *numInPorts = atoi(argv[currentArgIndex]);
    currentArgIndex++;
    *numOutPorts = atoi(argv[currentArgIndex]);
    currentArgIndex++;
    probInOutMatrix = new double[(*numInPorts)*(*numOutPorts)];

    for(int i=0; i<*numInPorts; i++) {
        for(int j=0; j<*numOutPorts; j++) {
            probInOutMatrix[i*(*numOutPorts)+j] = atof(argv[currentArgIndex]);
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
    double* probInOutMatrix = nullptr;
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
    vector<outPort> outPorts;

    for(int i=0; i<numOutPorts; i++) {
        outPorts.emplace_back(i, outProbs[i]);
    }

    double maxServiceTime = 0;
    double currServiceTime = 0;
    double intervalStart = 0, intervalEnd = 0;

    while(!arrivals.empty()) {
        int currPacketIndex = 0;
        intervalStart = arrivals.front().second;
        currPacketIndex = arrivals.front().first;

        arrivals.pop_front();

        if(arrivals.empty()) {
            intervalEnd = INT_MAX;
        } else {
            intervalEnd = arrivals.front().second;
        }

        int outPort = simulateOutPort(probInOutMatrix, currPacketIndex, numOutPorts);

        outPorts[outPort].insertPacket(intervalStart);

        for(int i=0; i<numOutPorts; i++) {
            currServiceTime = outPorts[i].deliverPackets(intervalStart, intervalEnd);

            if(currServiceTime > maxServiceTime) {
                maxServiceTime = currServiceTime;
            }
        }
    }

    double totalDuration = intervalStart + maxServiceTime;
    int totalDelivered = 0;
    int totalDropped = 0;
    double avgWaitTime = 0;
    double avgServiceTime = 0;

    for(int i=0; i<numOutPorts; i++) {
        totalDelivered += outPorts[i].deliveredPackets;
        totalDropped += outPorts[i].droppedPackets;
        avgWaitTime += outPorts[i].totalWaitTime;
        avgServiceTime += outPorts[i].totalServiceTime;
    }

    cout << totalDelivered << " ";

    for(int i=0; i<numOutPorts; i++) {
        cout << outPorts[i].deliveredPackets << " ";
    }

    cout << totalDropped << " ";

    for(int i=0; i<numOutPorts; i++) {
        cout << outPorts[i].droppedPackets << " ";
    }

    cout << totalDuration << " " << avgWaitTime/totalDelivered << " " << avgServiceTime/totalDelivered << endl;

    delete[] probInOutMatrix;
    delete[] inProb;
    delete[] outQueueSizes;
    delete[] outProbs;

    return 0;
}
