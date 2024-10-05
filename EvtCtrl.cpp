#include "EvtCtrl.h"
#include <MemoryUsage.h>


//STATIC INITIALIZATIONS
bool EvtCtrl::inClick = false;
uint8_t EvtCtrl::screenClickEventsCount = 0;
uint8_t EvtCtrl::transitoryClickEventsCount = 0;
ClickEvent** EvtCtrl::screenClickEvents = nullptr;
ClickEvent** EvtCtrl::transitoryClickEvents = nullptr;

static void EvtCtrl::addScreenEvent(ClickEvent* pEvent) {
  ClickEvent** newScreenClickEvents = new ClickEvent*[screenClickEventsCount + 1];    

  for (uint8_t i = 0; i < screenClickEventsCount; i++) {
    newScreenClickEvents[i] = screenClickEvents[i];
  }
  newScreenClickEvents[screenClickEventsCount] = pEvent;
  if (screenClickEvents != nullptr) {
    delete[] screenClickEvents;
  }
  screenClickEvents = newScreenClickEvents;
  screenClickEventsCount++;
  FREERAM_PRINT;
}

static bool EvtCtrl::checkClickEvent(const ClickEvent* ev, const int& x,const int& y) {
  //Serial.println(F("INIT EvtCtrl::checkClickEvent "));
  //Serial.println(String(id));
  bool result = false;
  //Serial.println("enabled "+String(x) + " " + String(y));
  if (ev != nullptr && ev->enabled) {
    //Serial.println("has component "+String(component->x)+","+String(component->y)+","+String(component->r1)+","+boolToString(component->isCircular())+","+String(component->r1)+ "," + String(component->getHeight())+","+String(abs(component->x - x))+","+String(abs(component->y - y)));      
    
    if (ev->type == 2 || ev->type == 3) { //circle | triangle
      if (abs(ev->x - x) <= ev->m1 && abs(ev->y - y) <= ev->m1) {
        result = true;
      }
    } else { //rectangle
      if (x >= ev->x && x <= ev->x + ev->m1
        && y >= ev->y && y <= ev->y + ev->m2
      ) {
        result = true;
      }
    } 
    
    if (result) {
      //Serial.println("clicked on conponent");
      if (ev->onClickCallback != nullptr) {
        //Serial.println("calling lambda callback");
        ev->onClickCallback->call();  // Chama a função encapsulada
      } else if (ev->staticOnClick != nullptr) {
        //Serial.println("calling static callback");
        ev->staticOnClick();
      }
    }
  }
  //Serial.println(F("END EvtCtrl::checkClickEvent "));
  return result;
};

static void EvtCtrl::checkClickedEvents(TSPoint point) {
  //Serial.println(F("INIT EvtCtrl::checkClickedEvents"));
  if (!inClick) {
    inClick = true;
    bool clicked = false;
    int px2 = map(point.y, TSCtrl::TS_MINX, TSCtrl::TS_MAXX, TSCtrl::tft.width(),0);
    int py2 = map(point.x, TSCtrl::TS_MINY, TSCtrl::TS_MAXY, TSCtrl::tft.height(),0);
    //Serial.println("clicked on " + String(point.x) + "," + String(point.y) + " > " +String(px2) + "," + String(py2) + " " + String(inClick));

    //loop troght event receptors array, checking if is point in area of element
    for (uint8_t i = 0; i < screenClickEventsCount; i++) {
      clicked = checkClickEvent(screenClickEvents[i],px2,py2);
      if (clicked) break;
    }
    if (!clicked) {
      for (uint8_t i = 0; i < transitoryClickEventsCount; i++) {
        clicked = checkClickEvent(transitoryClickEvents[i],px2,py2);
        if (clicked) break;
      }
    } 
    inClick = false;
  }
  //Serial.println(F("END EvtCtrl::checkClickedEvents"));   
}


static void EvtCtrl::clearAllEvents() {
  //Serial.println(F("INIT EvtCtrl::clearAllEvents"));
  for (uint8_t i = 0; i < screenClickEventsCount; i++) {
    if (screenClickEvents[i] != nullptr) {
      if (screenClickEvents[i]->onClickCallback != nullptr) {
        delete screenClickEvents[i]->onClickCallback;
      }
      delete screenClickEvents[i];
    }
  }
  delete[] screenClickEvents;
  screenClickEvents = nullptr;
  screenClickEventsCount = 0;
  for (uint8_t i = 0; i < transitoryClickEventsCount; i++) {
    if (transitoryClickEvents[i] != nullptr) {
      if (transitoryClickEvents[i]->onClickCallback != nullptr) {
        delete transitoryClickEvents[i]->onClickCallback;
      }
      delete transitoryClickEvents[i];
    }
  }
  delete[] transitoryClickEvents;
  transitoryClickEvents = nullptr;
  transitoryClickEventsCount = 0;
  FREERAM_PRINT;
  //Serial.println(F("END EvtCtrl::clearAllEvents"));   
}
