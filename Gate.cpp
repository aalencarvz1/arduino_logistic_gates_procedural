#include "Gate.h"
#include "Utils.h"

Gate::Gate(
  uint8_t pType,
  uint16_t pX,
  uint16_t pY,
  uint16_t pH,
  uint16_t pW,
  uint8_t pInputCount
) :
  type(pType),
  x(pX),
  y(pY),
  h(pH),
  w(pW),
  inputCount(pInputCount)
{
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
    delete[] connectedGates; //not delete elements, only array of pointers
  }
  if (connectedInputs != nullptr) {
    delete[] connectedInputs; //not delete elements, only array of pointers
  }
}