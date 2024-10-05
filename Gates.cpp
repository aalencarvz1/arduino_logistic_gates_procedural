#include "Gates.h"
#include <string.h>

const uint8_t GATES_NAMES_COUNT = 7; //BY PASS IGNORED
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

// Função para encontrar a próxima porta no array
const char* getNextGateName(const char* gateName) {
    for (int i = 0; i < GATES_NAMES_COUNT; i++) {
        if (strcmp(GATES_NAMES[i], gateName) == 0) {
            // Se for a última porta, retorna a primeira
            if (i == GATES_NAMES_COUNT - 1) {
                return GATES_NAMES[0];
            } else {
                // Senão, retorna a próxima porta
                return GATES_NAMES[i + 1];
            }
        }
    }
    // Caso o nome não seja encontrado, retorna NULL
    return NULL;
}

const char* getPrevGateName(const char* gateName) {
    for (int i = GATES_NAMES_COUNT-1; i >= 0; i--) {
        if (strcmp(GATES_NAMES[i], gateName) == 0) {
            // Se for a última porta, retorna a primeira
            if (i == 0) {
                return GATES_NAMES[GATES_NAMES_COUNT-1];
            } else {
                // Senão, retorna a anterior porta
                return GATES_NAMES[i - 1];
            }
        }
    }
    // Caso o nome não seja encontrado, retorna NULL
    return NULL;
}

void deleteGate(const Gate* g) {
  if (g != nullptr) {
    if (g->connectedGates != nullptr) {
      delete[] g->connectedGates; //not delete elements, only array of pointers
    }
    if (g->connectedInputs != nullptr) {
      delete[] g->connectedInputs; //not delete elements, only array of pointers
    }
  }
  delete g;
  g = nullptr;
}