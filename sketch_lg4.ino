#include "TSCtrl.h"
#include "ScreensCtrl.h"
#include "EvtCtrl.h"



void setup() {
  Serial.begin(9600);
  TSCtrl::init(); 
  ScreensCtrl::goTo(2);
}

void loop() {
  TSPoint p = TSCtrl::ts.getPoint();

  //stackoverflow credits: restore status of shared pins to avoid not responding tft call functions
  pinMode(TSCtrl::TS_XM,OUTPUT);
  pinMode(TSCtrl::TS_YP,OUTPUT);

  //clicked
  if (p.z > 0 && !EvtCtrl::inClick) {
    EvtCtrl::checkClickedEvents(p);
  }

  delay(100);
}
