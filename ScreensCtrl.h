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
  static void drawTutorialGatesScreen(TextInfo titleInfo, char* params[]);
  static void goTo(uint8_t screenId, char* params[] = nullptr);
  static void goBack();
};

#endif // SCREENSCTRL_H