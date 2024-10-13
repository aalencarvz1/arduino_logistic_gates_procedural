#include "ScreenTutorialGates.h"
#include "Gates.h"
#include "DrawCtrl.h"
#include "EvtCtrl.h"


//STATIC INITIALIZATIONS
static char* ScreenTutorialGates::currentGateName = GATES_NAMES[0];
static ClickEvent* ScreenTutorialGates::prevEv = nullptr;
static ClickEvent* ScreenTutorialGates::nextEv = nullptr;
static Gate* ScreenTutorialGates::currentGate = nullptr;
static bool ScreenTutorialGates::prevEnabled = false;
static bool ScreenTutorialGates::nextEnabled = false;
static TextInfo ScreenTutorialGates::titleInfo;

static void ScreenTutorialGates::freeMemory() {
  //Serial.println(F("INIT ScreenTutorialGates::freeMemory"));
  currentGateName = GATES_NAMES[0];
  if (currentGate != nullptr) {
    delete currentGate;
    currentGate = nullptr;
  }

  //events must have deleted by events controller
  prevEv = nullptr;
  nextEv = nullptr;
  prevEnabled = false;
  nextEnabled = false;
}

static void ScreenTutorialGates::drawNavigationButtons(){
  bool previousPrevEnabled = prevEnabled;
  bool previousNextEnabled = nextEnabled;
  prevEnabled = strcmp(currentGateName,GATES_NAMES[0]) != 0;
  nextEnabled = strcmp(currentGateName,GATES_NAMES[GATES_NAMES_COUNT-1]) != 0;  
  uint16_t color = TFT_YELLOW;
  void* onClick = nullptr;
  
  if (prevEnabled) {
    if (prevEv == nullptr) {
      onClick = []{
        ScreenTutorialGates::drawPrevGate();
      };
    } 
  } else if (previousPrevEnabled) {
    color = DEFAULT_BACKGROUND_COLOR;
  };
  if (prevEnabled && onClick != nullptr) {
    prevEv = DrawCtrl::drawClickable(
      onClick,
      nullptr,
      3,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+50,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2+25,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+50,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2-25,
      0,
      5,
      color
    );    
    EvtCtrl::addScreenEvent(prevEv);
  }; 

  if (!(prevEnabled && onClick != nullptr) && prevEnabled != previousPrevEnabled) {
    double x2=0;
    double y2=0;
    double r1=0;
    DrawCtrl::drawComponent(
      3,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+50,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2+25,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+50,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2-25,
      x2,
      y2,
      r1,
      5,
      color
    );
  };
  if (prevEv != nullptr) {
    prevEv->enabled = prevEnabled;
  }

  onClick = nullptr;
  if (nextEnabled) {
    if (nextEv == nullptr) {
      onClick = []{
        ScreenTutorialGates::drawNextGate();
      };
    } 
  } else if (previousNextEnabled) {
    color = DEFAULT_BACKGROUND_COLOR;
  };
  if (nextEnabled && onClick != nullptr) {
    nextEv = DrawCtrl::drawClickable(
      onClick,
      nullptr,
      3,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 60,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2-25,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 60,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2+25,
      0,
      5,
      color
    );    
    EvtCtrl::addScreenEvent(nextEv);
  }; 

  if (!(nextEnabled && onClick != nullptr) && nextEnabled != previousNextEnabled) {
    double x2=0;
    double y2=0;
    double r1=0;
    DrawCtrl::drawComponent(
      3,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 60,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2-25,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 60,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2+25,
      x2,
      y2,
      r1,
      5,
      color
    );
  };
  if (nextEv != nullptr) {
    nextEv->enabled = nextEnabled;
  }
}

static void ScreenTutorialGates::drawGate(const char* gateName){
  EvtCtrl::clearTransitoryEvents();
  if (currentGate != nullptr) {
    delete currentGate;
    currentGate = nullptr;    
  }
  //clear gate name
  TSCtrl::tft.fillRect(
    DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+5,
    titleInfo.y + titleInfo.h,
    100,
    20,
    DEFAULT_BACKGROUND_COLOR
  );

  TSCtrl::tft.fillRect(
    DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+5+80,
    titleInfo.y + titleInfo.h,
    300,
    TSCtrl::tft.height() - (DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + titleInfo.h + 10),
    DEFAULT_BACKGROUND_COLOR
  );

  TSCtrl::tft.setCursor(DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+5,titleInfo.y + titleInfo.h+5);
  TSCtrl::tft.setTextSize(2);
  TSCtrl::tft.setTextColor(DEFAULT_TEXT_COLOR);
  TSCtrl::tft.print(gateName);

  currentGate = new Gate(getGateIndex(gateName),150,250,150);
  initGateMeasurements(currentGate);
  DrawCtrl::drawGate(currentGate);
}

static void ScreenTutorialGates::drawPrevGate(){
  currentGateName = getPrevGateName(currentGateName);
  drawNavigationButtons();
  drawGate(currentGateName);
}

static void ScreenTutorialGates::drawNextGate(){
  currentGateName = getNextGateName(currentGateName);
  drawNavigationButtons();
  drawGate(currentGateName);
}

static void ScreenTutorialGates::draw(TextInfo pTitleInfo, char* params[]) {
  freeMemory();
  titleInfo = pTitleInfo;
  if (params != nullptr && params[0] != nullptr) {
    currentGateName = params[0];
  } else {
    currentGateName = GATES_NAMES[0];
  }
  currentGateName = getPrevGateName(currentGateName);
  drawNextGate();
}