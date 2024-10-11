#ifndef SCREENSCTRL_H
#define SCREENSCTRL_H

#include <stdint.h>
#include <StackArray.h>
#include "DrawCtrl.h"

struct ScreensCtrl{
  static StackArray <uint8_t> stack;
  static void drawGateButton(
    double pX,
    double pY,
    double pR,
    int col,
    int lin,
    double hSupSpace,
    int pColor,
    const char* pGateName
  );
  static void drawHomeScreenOptions(TextInfo titleInfo);
  static void drawTutorialScreenOptions(TextInfo titleInfo);
  static void drawPoitnsScreenOptions(TextInfo titleInfo);
  static void goTo(uint8_t screenId, char* params[] = nullptr,bool popCurrent = false);
  static void destroy(uint8_t screenId);  
  static void goBack();
};

#endif // SCREENSCTRL_H