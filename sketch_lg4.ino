#include "TSCtrl.h"
#include "ScreensCtrl.h"
#include "EvtCtrl.h"
#include "Gate.h"
#include "DrawCtrl.h"
#include "Gates.h"




void setup() {
  Serial.begin(9600);
  TSCtrl::init(); 
  randomSeed(analogRead(A0) + micros());
  ScreensCtrl::goTo(1);
  
  /*
  //gate draw test
  TSCtrl::tft.fillScreen(DEFAULT_BACKGROUND_COLOR);
  int gateSize = 150;
  Gate* g = new Gate(
    1,
    TSCtrl::tft.width()/2-gateSize/2,
    TSCtrl::tft.height() / 2 + gateSize/2 ,
    gateSize,
    gateSize+100,
    7
  );
  initGateMeasurements(g);
  DrawCtrl::drawGate(g);*/
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

  if (EvtCtrl::onTime != nullptr) {
    EvtCtrl::onTime->call();
  }

  delay(100);
}
