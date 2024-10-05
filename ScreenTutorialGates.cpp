#include "ScreenTutorialGates.h"
#include "Gates.h"
#include "DrawCtrl.h"
#include "EvtCtrl.h"


//STATIC INITIALIZATIONS
static char* ScreenTutorialGates::currentGateName = GATES_NAMES[0];
static ClickEvent* ScreenTutorialGates::prevEv = nullptr;
static ClickEvent* ScreenTutorialGates::nextEv = nullptr;
static bool ScreenTutorialGates::prevEnabled = false;
static bool ScreenTutorialGates::nextEnabled = false;

static void ScreenTutorialGates::init() {
  currentGateName = GATES_NAMES[0];
  prevEv = nullptr;
  nextEv = nullptr;
  prevEnabled = false;
  nextEnabled = false;
}

static void ScreenTutorialGates::drawNavigationButtons(){
  Serial.println(currentGateName);
  bool previousPrevEnabled = prevEnabled;
  bool previousNextEnabled = nextEnabled;
  prevEnabled = strcmp(currentGateName,GATES_NAMES[0]) != 0;
  nextEnabled = strcmp(currentGateName,GATES_NAMES[GATES_NAMES_COUNT-1]) != 0;  
  Serial.println(boolToString(prevEnabled)+","+boolToString(nextEnabled));
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
    Serial.println("creating prev event");
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
    Serial.println("drawing prev button");
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
    Serial.println("creating next event");
    nextEv = DrawCtrl::drawClickable(
      onClick,
      nullptr,
      3,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 50,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2-25,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 50,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2+25,
      0,
      5,
      color
    );    
    EvtCtrl::addScreenEvent(nextEv);
  }; 

  if (!(nextEnabled && onClick != nullptr) && nextEnabled != previousNextEnabled) {
    Serial.println("drawing prev button");
    double x2=0;
    double y2=0;
    double r1=0;
    DrawCtrl::drawComponent(
      3,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 50,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2-25,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 50,
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

static void ScreenTutorialGates::drawPrevGate(){
  Serial.println("going back gate");
  currentGateName = getPrevGateName(currentGateName);
  drawNavigationButtons();
  draw gate
}

static void ScreenTutorialGates::drawNextGate(){
  Serial.println("going next gate");
  currentGateName = getNextGateName(currentGateName);
  drawNavigationButtons();
  draw gate
}

static void ScreenTutorialGates::draw(TextInfo titleInfo, char* params[]) {
  init();
  if (params != nullptr && params[0] != nullptr) {
    currentGateName = params[0];
  } else {
    currentGateName = GATES_NAMES[0];
  }
  currentGateName = getPrevGateName(currentGateName);
  drawNextGate();
}