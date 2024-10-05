#include "Gates.h"

const char* GATES_NAMES[] = {"AND", "OR", "NOT", "NAND", "NOR", "XOR", "XNOR", "BYPASS",nullptr};

int8_t getGateIndex(char* pGateName) {
  int8_t result = -1;
  int8_t i = 0;
  while(GATES_NAMES[i] != nullptr) {
    if (GATES_NAMES[i] == pGateName) {
      result = i;
      break;
    };
    i++;
  };
  return result;
}

void deleteGate(Gate* g) {
  if (g != nullptr) {
    if (g->connectedGates != nullptr) {
      delete[] g->connectedGates; //not delete elements, only array of pointers
    }
    if (g->connectedInputs != nullptr) {
      delete[] g->connectedInputs; //not delete elements, only array of pointers
    }
  }
}