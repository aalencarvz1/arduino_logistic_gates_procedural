#ifndef GATES_H
#define GATES_H

#include <stdint.h>
#include "Gate.h"

extern const uint8_t GATES_NAMES_COUNT;
extern const char* GATES_NAMES[];

int8_t getGateIndex(char* pGateName);
const char* getNextGateName(const char* gateName);
const char* getPrevGateName(const char* gateName);
void initGateMeasurements(Gate* g);
void invertGateInput(Gate* gate,uint8_t inputIndex);
bool calcOutputState(Gate* g);
void addGateToGateArray(Gate**& pGateArray, Gate*& pGate, uint8_t& currentCount);
void addConnectedGate(Gate*& originOutput,Gate*& destinyGate,uint8_t destinyInputIndex);

#endif