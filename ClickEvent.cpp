#include "ClickEvent.h"
#include <Arduino.h>

static uint16_t ClickEvent::currentId = 0;

ClickEvent::ClickEvent()
  : id(currentId) {
  Serial.println("created event " + String(id));
  currentId++;
}

ClickEvent::~ClickEvent() {
  if (onClickCallback != nullptr) {
    delete onClickCallback;
    onClickCallback = nullptr;
  }
  Serial.println("destroyed event " + String(id));
}