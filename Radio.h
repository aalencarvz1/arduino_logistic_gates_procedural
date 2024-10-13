#ifndef RADIO_H
#define RADIO_H

#include "ClickEvent.h"

struct Radio {
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  char* text = nullptr;
  bool selected = false;
  ClickEvent* ev = nullptr;
};

#endif;