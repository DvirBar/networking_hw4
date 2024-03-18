#include <iostream>

using namespace std;

void processArgs(char *argv[], int* maxArrivalTime, int* numInPorts, int* numOutPorts,
                 int* probInOutMatrix, int* inProb, int* outQueueSizes, int* outProbs) {
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

    inProb = new int[*numInPorts];

    for(int i=0; i<*numInPorts; i++) {
        inProb[i] = atoi(argv[currentArgIndex]);
        currentArgIndex++;
    }

    outQueueSizes = new int[*numOutPorts];

    for(int i=0; i<*numOutPorts; i++) {
        outQueueSizes[i] = atoi(argv[currentArgIndex]);
        currentArgIndex++;
    }

    outProbs = new int[*numOutPorts];

    for(int i=0; i<*numOutPorts; i++) {
        outProbs[i] = atoi(argv[currentArgIndex]);
        currentArgIndex++;
    }
}

int main(int argc, char *argv[]) {
    int maxArrivalTime, numInPorts, numOutPorts;
    int* probInOutMatrix = nullptr;
    int* inProb = nullptr;
    int* outQueueSizes = nullptr;
    int* outProbs = nullptr;

    processArgs(argv, &maxArrivalTime, &numInPorts, &numOutPorts,
                probInOutMatrix, inProb, outQueueSizes, outProbs);


    delete[] probInOutMatrix;
    delete[] inProb;
    delete[] outQueueSizes;
    delete[] outProbs;

    return 0;
}
