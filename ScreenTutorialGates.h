#ifndef SCREENTUTORIALGATES_H
#define SCREENTUTORIALGATES_H

#include "ClickEvent.h"
#include "DrawCtrl.h"

struct ScreenTutorialGates {
  static char* currentGateName;
  static bool prevEnabled;
  static bool nextEnabled;
  static ClickEvent* nextEv;
  static ClickEvent* prevEv;
  static void init();
  static void draw(TextInfo titleInfo, char* params[]);
  static void drawNavigationButtons();
  static void drawPrevGate();
  static void drawNextGate();
};



#endif