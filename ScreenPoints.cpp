#include "ScreenPoints.h"
#include "TSCtrl.h"
#include "ScreenPointsGates.h"
#include "EvtCtrl.h"
#include "Colors.h"
#include "ScreensCtrl.h"

static uint8_t ScreenPoints::currentSelectedIndex = 0;
static uint8_t ScreenPoints::optionsCount = 7;
static Radio** ScreenPoints::options = nullptr;


static void ScreenPoints::freeMemory() {
  if (options != nullptr) {
    for(uint8_t i = 0; i < optionsCount; i++) {
      if (options[i] != nullptr) {
        delete options[i];
      }
    }
    delete[] options;
    options = nullptr;
  }
}

static void ScreenPoints::initRadios(TextInfo titleInfo) {
  freeMemory();
  options = new Radio*[optionsCount]{nullptr};
  for(int i = 0; i < optionsCount; i++) {
    options[i] = new Radio();
    options[i]->x = DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + 45;
    options[i]->y = titleInfo.y+titleInfo.h+40+i*30;
    options[i]->w = 300;
    options[i]->h = 25;
    options[i]->selected = currentSelectedIndex == i;
    options[i]->ev = new ClickEvent();
    options[i]->ev->type = 1; //rectangle
    options[i]->ev->x = options[i]->x-35;
    options[i]->ev->y = options[i]->y;
    options[i]->ev->m1 = options[i]->w;
    options[i]->ev->m2 = options[i]->h;
    auto f = [i,options,optionsCount](){      
      DrawCtrl::selectRadio(i,options,optionsCount);
    };
    options[i]->ev->onClickCallback = new LambdaCallback<decltype(f)>(f);
    EvtCtrl::addScreenEvent(options[i]->ev);
  }
  options[0]->text = "1 - Facil";
  options[1]->text = "2 - Facil";
  options[2]->text = "3 - Intermediario";
  options[3]->text = "4 - Intermediario";
  options[4]->text = "5 - Intermediario";
  options[5]->text = "6 - Avancado";
  options[6]->text = "7 - Avancado";
}

static void ScreenPoints::initPontuation() {
  for(uint8_t i = 0; i < optionsCount; i++) {   
    if (options[i]->selected) {
      ScreenPointsGates::currentLevel = i+1;
    }
  }
  ScreensCtrl::goTo(30,nullptr,true);
}

static void ScreenPoints::draw(TextInfo titleInfo, char* params[]) {  
  initRadios(titleInfo);
  TSCtrl::tft.setCursor(DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + 5,titleInfo.y+titleInfo.h+5);
  TSCtrl::tft.setTextSize(2);
  TSCtrl::tft.setTextColor(DEFAULT_TEXT_COLOR);
  TSCtrl::tft.print(F("Iniciar no nivel:"));
  for(uint8_t i = 0; i < optionsCount; i++) {   
    DrawCtrl::drawRadio(options[i]);
  }

  EvtCtrl::addScreenEvent(DrawCtrl::drawClickable(
    []{
      ScreenPoints::initPontuation();
    },
    nullptr,
    1,
    TSCtrl::tft.width()/2-60,
    TSCtrl::tft.height()-DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN-25,
    120,
    25,
    0,
    3,
    Colors::GREEN,
    true,
    "Iniciar"
  ));
}