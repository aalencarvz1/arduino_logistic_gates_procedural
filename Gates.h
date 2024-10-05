#ifndef GATES_H
#define GATES_H

#include <stdint.h>
#include "Gate.h"

extern const char* GATES_NAMES[];

int8_t getGateIndex(char* pGateName);
void deleteGate(Gate* g);

#endif