#include "Circuit.h"
#include "DrawCtrl.h"
#include "Gates.h"
#include "TSCtrl.h"

Circuit::Circuit(
  uint8_t pGateLevelCount,
  uint8_t pGateIds[]
) : 
  gateLevelCount(pGateLevelCount)
{
  if (pGateIds != nullptr) {
    createGates(pGateIds);
  }
};

Circuit::~Circuit(){
  if (gates != nullptr) {
    for(uint8_t i = 0; i < gateLevelCount; i++) {
      delete gates[i];
    }
    delete[] gates;
    gates = nullptr;
  }
}

/*void Circuit::addGate(Gate* pGate) {
  Gate** newGates = 
}*/

Gate* Circuit::getNextGateInputConnector(uint8_t& pOutputInpuIndex) {
  Gate* result = nullptr;
  if (gates != nullptr) {
    for (uint8_t i = 0; i < gateCount; i++) {
      if (gates[i] != nullptr) {
        for (uint8_t j = 0 ; j < gates[i]->inputCount; j++) {
          Serial.println("xxxxx searching in gate " + String(i) + ", inputIndex "+String(j));
          if (!getBit(gates[i]->packedInputsWithOtputs,j)) {
            Serial.println("  found ");
            pOutputInpuIndex = j;
            result = gates[i];
            break;
          }
        }
        if (pOutputInpuIndex != 255) break;
      }
    }
  }
  return result;
}

Gate* Circuit::createGate(
  uint8_t pGateId,
  bool draw,
  int pX,
  int pY,
  double pSize,
  double pWidth,
  Gate* pOutputGate,
  uint8_t pOutputInpuIndex
) {
  Serial.println(F("INIT Circuit::createGate"));
  Gate* g = nullptr;
  uint8_t currentCircuitLevel = 0;

  if (pOutputGate == nullptr) {
    pOutputGate = getNextGateInputConnector(pOutputInpuIndex);
  } else if (pOutputInpuIndex == 255) {
    getNextGateInputConnector(pOutputInpuIndex);
  }  

  Serial.println("has gate output "+boolToString(pOutputGate != nullptr) + "," + String(pOutputInpuIndex));

  if (pOutputGate != nullptr) {
    setBit(pOutputGate->packedInputsWithOtputs,pOutputInpuIndex,true);
    currentCircuitLevel = pOutputGate->currentCircuitLevel + 1;
  }
  if (pSize == 0) {
    pSize = ((DEFAULT_WINDOW_CONTAINER_H /*- DEFAULT_WINDOW_CONTAINER_SUBTITLE_H*/) / gateLevelCount) * DEFAULT_GATE_VERTICAL_SIZE_PERC;
  }
  if (pWidth == 0) {
    pWidth = pSize;
  }
  double lastLevelWidth = (pow(2,gateLevelCount - 1) * pWidth) + ((pow(2,gateLevelCount - 1) - 1) * (pWidth * DEFAULT_GATE_VERTICAL_SIZE_PERC ));

  if (lastLevelWidth > DEFAULT_WINDOW_CONTAINER_W) {
    lastLevelWidth = DEFAULT_WINDOW_CONTAINER_W;
    pWidth = (lastLevelWidth / pow(2,gateLevelCount - 1));
    pWidth = pWidth - pWidth * 0.2; //minimal space
  }

  double currentGateSapce = lastLevelWidth / pow(2,currentCircuitLevel);
  Serial.println("lastLevelWidth "+String(lastLevelWidth) + ", currentGateSapce " + String(currentGateSapce));

  if (pX == 0) {
    if (gates == nullptr || gates[0] == nullptr) {
        pX = TSCtrl::tft.width() / 2 - pWidth / 2;
    } else if (pOutputGate != nullptr) {            
      double levelGateWidth = pOutputGate->inputCount * currentGateSapce;
      double x1 = pOutputGate->x + pOutputGate->w / 2 - levelGateWidth / 2;
      x1 = x1 + currentGateSapce / 2 - pWidth / 2;
      pX = x1 + pOutputInpuIndex * currentGateSapce;//(pWidth + gateSpacing); 
    }
  }

  if (pY == 0) {
    if (gates == nullptr || gates[0] == nullptr) {
      pY = DEFAULT_WINDOW_CONTAINER_AFTER_SUBTITLE_Y + pSize + pSize * DEFAULT_GATE_INPUT_CONNECTOR_SIZE_PERC * 2;
    } else if (pOutputGate != nullptr) {
      pY = pOutputGate->y + pOutputGate->connectorSize * 2 + pSize;  //@todo implement detect current input wich is connectec output this gate to calculate x, implement on set inputoutpu, set also gateoutput on inpu
    }
  }

  Serial.println(String(pX)+","+String(pY)+","+String(pSize)+ "," + String(pWidth));

  g = new Gate(
    pGateId,
    pX,
    pY,
    pSize,
    pWidth
  );
  initGateMeasurements(g);
  
  g->currentCircuitLevel = currentCircuitLevel;

  if (pOutputGate != nullptr && pOutputInpuIndex != 255) {
    addConnectedGate(g,pOutputGate,pOutputInpuIndex);
  }

  setBit(g->packedFlags,7,false);//visible output
  if (g->currentCircuitLevel < gateLevelCount -1) {
    setBit(g->packedFlags,6,false);//visible inputs    
  }

  addGateToGateArray(gates,g,gateCount);
  Serial.println("yyyyyyyyyyyyyyyy"+String(gateCount));
  if (draw) {
    DrawCtrl::drawGate(g);
  }
  Serial.println(F("END Circuit::createGate"));
  return g;
}

void Circuit::createGates(uint8_t pGateIds[]) {
  //Serial.println(F("INIT GatesCircuit::createGates"));
  if (pGateIds != nullptr) {
    Gate* g = nullptr;
    uint8_t i = 0;
    while(pGateIds[i] != 255) {
      Serial.println("creating " + String(i) + " " + String(pGateIds[i]));
      if (pGateIds[i] == 255) break;
      g = createGate(pGateIds[i]);
      /*if (g != nullptr) {
        DrawCtrl::drawGate(g);
      }*/
      i++;
    }
  }
  //Serial.println(F("END GatesCircuit::createGates"));
}