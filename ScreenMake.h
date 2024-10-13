
#ifndef SCREENMAKE_H
#define SCREENMAKE_H

#include "ClickEvent.h"
#include "DrawCtrl.h"
#include "Circuit.h"

struct ScreenMake {
  static TextInfo titleInfo;
  static int buttonSize;
  static Circuit* currentCircuit;
  static void freeMemory();
  static void addGate(uint8_t pGateId);
  static void drawGateButton(uint8_t pGateId, int x, int y);
  static void draw(TextInfo titleInfo, char* params[]);
};

#endif