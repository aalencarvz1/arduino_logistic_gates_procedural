#ifndef SCREENPOINTS_H
#define SCREENPOINTS_H

#include "DrawCtrl.h"
#include "Radio.h"



struct ScreenPoints {
  static uint8_t currentSelectedIndex;
  static uint8_t optionsCount;
  static Radio** options;
  static void freeMemory();
  static void initRadios(TextInfo titleInfo);
  static void initPontuation();
  static void draw(TextInfo titleInfo, char* params[] = nullptr);
};

#endif