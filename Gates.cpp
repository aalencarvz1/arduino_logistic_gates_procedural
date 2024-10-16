#include "Gates.h"
#include <MemoryUsage.h>
#include <string.h>
#include "DrawCtrl.h"

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

void initGateMeasurements(Gate* g) {
  if (g != nullptr) {
    g->connectorSize = g->h * DEFAULT_GATE_INPUT_CONNECTOR_SIZE_PERC;
    g->notRadius = 0;
    if (getBit(g->packedFlags,8)) {//hasNot
      g->notRadius = g->h * DEFAULT_GATE_NOT_RADIUS_PERC;
      if (g->notRadius > DEFAULT_GATE_MAX_NOT_RADIUS) {
        g->notRadius = DEFAULT_GATE_MAX_NOT_RADIUS;
      }
      g->h -= g->notRadius;
    }
    g->inputRadius = g->w * DEFAULT_GATE_INPUT_RADIUS_PERC;
    if (g->inputRadius > DEFAULT_GATE_MAX_INPUT_BUTTON_RADIUS) {
      g->inputRadius = DEFAULT_GATE_MAX_INPUT_BUTTON_RADIUS;
    } else if (g->inputRadius < DEFAULT_GATE_MIN_INPUT_BUTTON_RADIUS) {
      g->inputRadius = DEFAULT_GATE_MIN_INPUT_BUTTON_RADIUS;
    }
    
    g->firstInputX = g->x + g->w * DEFAULT_GATE_INPUT_CONNECTOR_MARGIN_PERC;  
    g->inputSpaceBetwenn = 0;
    if (g->inputCount == 1) {
      g->firstInputX = g->x + g->w / 2;
    } else {
      g->inputSpaceBetwenn = (g->w - g->w * DEFAULT_GATE_INPUT_CONNECTOR_MARGIN_PERC * 2) / (g->inputCount-1);
    }
    g->firstInputY = g->y+g->connectorSize;
    g->outputX = g->x + g->w / 2.0;
    g->outputY = g->y - g->h - g->connectorSize - g->notRadius;
  }
}

bool calcOutputState(Gate* g) {
  bool outputState = false;
  if (g != nullptr) {
    switch(g->type) {
      case 1://or
      case 4://nor
        for (uint8_t i = 0; i < g->inputCount; i++) {
          outputState = getBit(g->packedInputs,i); 
          if (outputState) {
            break;
          };       
        }
        break;
      case 2://not
        outputState = getBit(g->packedInputs,0);
        break;
      case 5://xor
      case 6://xnor
        for (uint8_t i = 0; i < g->inputCount; i++) {
          if (outputState && getBit(g->packedInputs,i)) {
            outputState = false;
            break;
          } else if (!outputState) {            
            outputState = getBit(g->packedInputs,i);
          } 
        }
        break;
      default: //and, nand, bypass
        outputState = true;
        for (uint8_t i = 0; i < g->inputCount; i++) {
          outputState = outputState && getBit(g->packedInputs,i); 
          if (!outputState) {
            break;
          };       
        }
    }
    outputState = getBit(g->packedFlags,8) ? !outputState : outputState; //8-hasNot
    setBit(g->packedInputs,7,outputState);
  }

  return outputState;
}

/*
* method called when user click on input of gate
* invert current state of input and recalculate gate output state (bit 8 of packed inputs)
* redraw input clicked and output state if changed
*/
void setGateInputState(Gate*& gate,uint8_t inputIndex, bool newState) {
  if (gate != nullptr) {
    bool currentInputState = getBit(gate->packedInputs,inputIndex);
    if (currentInputState != newState) {
      setBit(gate->packedInputs,inputIndex,newState);
      if (getBit(gate->packedFlags,6)) {//visible inputs
        TSCtrl::tft.fillCircle(
          gate->firstInputX + gate->inputSpaceBetwenn * inputIndex, 
          gate->firstInputY, 
          gate->inputRadius,
          newState ? DEFAULT_GATE_INPUT_ON_COLOR : DEFAULT_GATE_INPUT_OFF_COLOR
        );
      }
      bool currentOutputState = getBit(gate->packedInputs,7);
      calcOutputState(gate);    
      if (currentOutputState != getBit(gate->packedInputs,7)) {
        DrawCtrl::drawGateOutputButton(gate);
        if (gate->connectedGates != nullptr && gate->connectedGatesQty > 0) {
          for(uint8_t i = 0; i < gate->connectedGatesQty; i++) {
            for(uint8_t j = 0; j < 8; j++) {
              if (getBit(gate->connectedInputs[i],j)) {
                setGateInputState(gate->connectedGates[i],j,getBit(gate->packedInputs,7));
              }
            }
          }
        }
      }
    }
  }
}


/*
* method called when user click on input of gate
* invert current state of input and recalculate gate output state (bit 8 of packed inputs)
* redraw input clicked and output state if changed
*/
void invertGateInput(Gate* gate,uint8_t inputIndex) {
  if (gate != nullptr) {
    setGateInputState(gate,inputIndex,!getBit(gate->packedInputs,inputIndex));    
  }
}

void addGateToGateArray(Gate**& pGateArray, Gate*& pGate, uint8_t& currentCount) {
  // Alocar um novo array com espaço adicional para o novo elemento
  Gate** newGateArray = new Gate*[currentCount + 1];
  
  // Copiar os elementos antigos para o novo array
  for (uint8_t i = 0; i < currentCount; i++) {
    newGateArray[i] = pGateArray[i];
  }
  
  // Adicionar o novo gate no final do array
  newGateArray[currentCount] = pGate;
  
  // Liberar a memória antiga se existir
  if (pGateArray != nullptr) {
    delete[] pGateArray;
  }
  
  // Atualizar o ponteiro do array antigo para o novo
  pGateArray = newGateArray;
  
  // Incrementar a contagem de gates
  currentCount++;
}


void addConnectedGate(Gate*& originOutput,Gate*& destinyGate,uint8_t destinyInputIndex) {    
  addGateToGateArray(originOutput->connectedGates,destinyGate,originOutput->connectedGatesQty);
    
  uint8_t* newConnectedGatesInputs = new uint8_t[originOutput->connectedGatesQty];    
  for (uint8_t i = 0; i < originOutput->connectedGatesQty-1; i++) {
    newConnectedGatesInputs[i] = originOutput->connectedInputs[i];
  }
  uint8_t newDestinyInputIndexs = 0b00000000;
  setBit(newDestinyInputIndexs,destinyInputIndex,true);

  setGateInputState(destinyGate,destinyInputIndex,calcOutputState(originOutput));

  newConnectedGatesInputs[originOutput->connectedGatesQty-1] = newDestinyInputIndexs;
  if (originOutput->connectedInputs != nullptr) {
    delete[] originOutput->connectedInputs;
  }
  originOutput->connectedInputs = newConnectedGatesInputs;


  //originOutput->connectedGatesQty++;

  //FREERAM_PRINT;
}

