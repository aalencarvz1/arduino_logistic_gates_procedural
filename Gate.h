#ifndef GATE_H
#define GATE_H

#include <stdint.h>


 /*
  //                        bit positions
  //                       1111110000000000
  //                       5432109876543210  
	uint16_t packedFlags = 0b0000000011111111;
  packedFlags =
    0 = bool vertical = true; 
    1 = bool hasInputConnectors = true;
    2 = bool inputConnectorsIsVisibles = true;        
    3 = bool hasOutputConnector = true;    
    4 = bool outputConnectorIsVisible = true;    
    5 = bool hasInputButtons = true; 
    6 = bool visibleInputs = true;
    7 - bool visibleOutput = true;
    8 - bool hasNot = false;    
    9 - bool exclusive = false;            
*/


struct Gate {
	uint8_t type = 0;
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t h = 0;
  uint16_t w = 0;
  uint8_t inputCount = 2;
	int16_t firstInputX = 0;
	int16_t firstInputY = 0;
	uint8_t inputSpaceBetwenn = 0;
  uint8_t inputRadius = 0;
  uint8_t notRadius = 0;
  uint8_t connectorSize = 0;
	int16_t outputX = 0;
	int16_t outputY = 0;

  //                        bit positions
  //                       1111110000000000
  //                       5432109876543210  
	uint16_t packedFlags = 0b0000000011111111;
	uint8_t packedInputs = 0b00000000;	

  

	Gate** connectedGates = nullptr; 
	uint8_t* connectedInputs = nullptr;
	uint8_t connectedGatesQty = 0;
  
  //represent level when this gate is part of cirtuic, base on head gate as 0
  uint8_t currentCircuitLevel = 0;
  
  //represent index of inputs wich has output connected to this to use in circuits
  uint8_t packedInputsWithOtputs = 0b00000000;	
  Gate(
    uint8_t pType = 0,
    uint16_t pX = 0,
    uint16_t pY = 0,
    uint16_t pH = 0,
    uint16_t pW = 0,
    uint8_t pInputCount = 2
  );
  ~Gate();

};




#endif