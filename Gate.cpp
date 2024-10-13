#include "Gate.h"
#include "Utils.h"

static uint16_t Gate::currentId = 0;

Gate::Gate(
  uint8_t pType,
  uint16_t pX,
  uint16_t pY,
  uint16_t pH,
  uint16_t pW,
  uint8_t pInputCount
) :
  id(currentId),
  type(pType),
  x(pX),
  y(pY),
  h(pH),
  w(pW),
  inputCount(pInputCount)
{
  Serial.println("created gate id "+String(id));
  currentId++;
  if (w == 0) {
    w = h; 
  }
  if (pType == 2 || pType == 7) { //not,bypass
    inputCount = 1;
  }
  if (pType == 2 || pType == 3 || pType == 4 || pType == 6) {
    setBit(packedFlags,8,true);//negation
  }
  if (pType == 5 || pType == 6) {
    setBit(packedFlags,9,true);//exclusive
  }
}

Gate::~Gate() {
  if (connectedGates != nullptr) {
    for(int i = 0; i < connectedGatesQty; i++) {
      connectedGates[i] = nullptr;
    }
    delete[] connectedGates; //not delete elements, only array of pointers
    //delete connectedGates;
    connectedGates = nullptr;
  }
  if (connectedInputs != nullptr) {
    for(int i = 0; i < connectedGatesQty; i++) {
      connectedInputs[i] = 0;
    }
    delete[] connectedInputs; //not delete elements, only array of pointers
    //delete connectedInputs;
    connectedInputs = nullptr;
  }
  connectedGatesQty = 0;
  Serial.println("destroyed gate id "+String(id));
}