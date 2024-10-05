#ifndef GATES_H
#define GATES_H

#include <stdint.h>
#include "Gate.h"

extern const uint8_t GATES_NAMES_COUNT;
extern const char* GATES_NAMES[];

int8_t getGateIndex(char* pGateName);
const char* getNextGateName(const char* gateName);
const char* getPrevGateName(const char* gateName);
void deleteGate(const Gate* g);

#endif