#ifndef EVTCTRL_H
#define EVTCTRL_H

#include <stdint.h>
#include "ClickEvent.h"
#include "TSCtrl.h"

struct EvtCtrl {
  static bool inClick; //AVOID LOOP ON DISPATH MULTIPLE CLICKS AT SOME TIME
  static uint8_t screenClickEventsCount;
  static uint8_t transitoryClickEventsCount;
  static ClickEvent** screenClickEvents;
  static ClickEvent** transitoryClickEvents;

  static void addEvent(ClickEvent* pEvent,ClickEvent** pList);
  static void addScreenEvent(ClickEvent* pEvent);
  static void addTransitoryEvent(ClickEvent* pEvent);
  static bool checkClickEvent(const ClickEvent* ev, const int& x,const int& y);
  static void checkClickedEvents(TSPoint point);
  static void clearScreenEvents();
  static void clearTransitoryEvents();
  static void clearAllEvents();
};

#endif // EVTCTRL_H