#ifndef SCREENPOINTS_H
#define SCREENPOINTS_H

#include "DrawCtrl.h"


struct Radio {
  int x = 0;
  int y = 0;
  char* text = nullptr;
  bool selected = false;
  ClickEvent* ev = nullptr;
};

struct ScreenPoints {
  static uint8_t currentSelectedIndex;
  static uint8_t optionsCount;
  static Radio** options;
  static void drawRadio(Radio* option);
  static void freeMemory();
  static void initRadios(TextInfo titleInfo);
  static void selectOption(uint8_t optionIndex);
  static void initPontuation();
  static void draw(TextInfo titleInfo, char* params[] = nullptr);
};

#endif