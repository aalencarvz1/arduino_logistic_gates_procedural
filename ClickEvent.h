#ifndef CLICKEVENT_H
#define CLICKEVENT_H

#include <stdint.h>
#include "LambdaCallback.h"

struct ClickEvent{
  static uint16_t currentId;
  uint16_t id = 0;
	uint8_t type = 0; //[1-rect,2-circle,3-triangle]
	int16_t x=0;
  int16_t y=0;
  int16_t m1=0;
  int16_t m2=0;
  bool enabled = true;
	void (*staticOnClick)() = nullptr;
  ICallback* onClickCallback = nullptr;  // Ponteiro para o callback
  ClickEvent();
  ~ClickEvent();
};

#endif // CLICKEVENT_H