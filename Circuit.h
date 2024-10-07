#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "Gate.h"
#define DEFAULT_CIRCUIT_GATE_LEVEL_COUNT 2
#define DEFAULT_GATE_VERTICAL_SIZE_PERC 0.5

struct Circuit {
  uint8_t gateLevelCount;
  Gate** gates = nullptr;
  Circuit(
    uint8_t pGateLevelCount,
    uint8_t pGateIds[] = nullptr
  );
  ~Circuit();
  Gate* getNextGateInputConnector(uint8_t& pOutputInpuIndex);
  Gate* createGate(
    uint8_t pGateId,
    int pX = 0,
    int pY = 0,
    double pSize = 0,
    double pWidth = 0,
    Gate* pOutputGate = nullptr,
    uint8_t pOutputInpuIndex = 255
  );
  void createGates(uint8_t pGateIds[]);
};

#endif //CIRCUIT_H