#ifndef SCREENTIME_H
#define SCREENTIME_H

#include "DrawCtrl.h"
#include "Radio.h"

struct ScreenTime {
  static uint8_t currentSelectedIndex;
  static uint8_t optionsCount;
  static uint8_t timeOptionsCount;
  static Radio** options;
  static Radio** timeOptions;
  static void freeMemory();
  static void initRadios(TextInfo titleInfo);
  static void initTime();
  static void draw(TextInfo titleInfo, char* params[] = nullptr);
};

#endif