
#ifndef SCREENPOINTSGATES_H
#define SCREENPOINTSGATES_H

#include "ClickEvent.h"
#include "DrawCtrl.h"
#include "Circuit.h"

#define DEFAULT_PHASE_TIME_INTERVAL 200

struct ScreenPointsGates {
  static bool nextEnabled;
  static ClickEvent* confirmEvent;
  static ClickEvent* nextEv;
  static uint8_t currentLevel;
  static uint8_t currentPhase;
  static uint8_t currentPontuation;  
  static double gateX;
  static double gateY;
  static double gateSize;  
  static double gateWidth;  
  static char* currentGateName;
  static TextInfo titleInfo;
  static TextInfo subTitleInfo;
  static Gate* currentGate;
  static Circuit* currentCircuit;
  static bool initialState;
  static bool updateInitialState;
  static void freeMemory();
  static void drawConfirmButton();
  static void drawCurrentPontuation();
  static void drawLevelAndPhase();
  static void clearGateSapce();
  static void clearAllSpaces();
  static void drawGateLevel(bool pClearGateSpace = true);
  static void drawCircuitLevel(bool pClearSpaces = true);
  static void drawNextPhase(bool pClearSpaces = true);
  static void draw(TextInfo titleInfo, char* params[]);
  static void incPhase();
  static void drawResult(bool result, bool clearOnly = false);
  static void confirm();
  static void drawNavigationButtons();
  
  //static void drawGate(const char* gateName);
  
  
};



#endif