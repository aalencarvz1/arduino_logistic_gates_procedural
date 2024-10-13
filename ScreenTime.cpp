#include "ScreenTime.h"
#include "TSCtrl.h"
#include "ScreenTimeGates.h"
#include "EvtCtrl.h"
#include "Colors.h"
#include "ScreensCtrl.h"

static uint8_t ScreenTime::currentSelectedIndex = 0;
static uint8_t ScreenTime::optionsCount = 7;
static uint8_t ScreenTime::timeOptionsCount = 7;
static Radio** ScreenTime::options = nullptr;
static Radio** ScreenTime::timeOptions = nullptr;


static void ScreenTime::freeMemory() {
  if (options != nullptr) {
    for(uint8_t i = 0; i < optionsCount; i++) {
      if (options[i] != nullptr) {
        delete options[i];
      }
    }
    delete[] options;
    options = nullptr;
  }

  if (timeOptions != nullptr) {
    for(uint8_t i = 0; i < timeOptionsCount; i++) {
      if (timeOptions[i] != nullptr) {
        delete timeOptions[i];
      }
    }
    delete[] timeOptions;
    timeOptions = nullptr;
  }
}

static void ScreenTime::initRadios(TextInfo titleInfo) {
  freeMemory();
  options = new Radio*[optionsCount]{nullptr};
  for(int i = 0; i < optionsCount; i++) {
    options[i] = new Radio();
    options[i]->x = DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + 45;
    options[i]->y = titleInfo.y+titleInfo.h+40+i*30;
    options[i]->w = 250;
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

  timeOptions = new Radio*[timeOptionsCount]{nullptr};
  for(int i = 0; i < timeOptionsCount; i++) {
    timeOptions[i] = new Radio();
    timeOptions[i]->x = DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + 45 + options[0]->w + 30;
    timeOptions[i]->y = titleInfo.y+titleInfo.h+40+i*30;
    timeOptions[i]->w = 100;
    timeOptions[i]->h = 25;
    timeOptions[i]->selected = currentSelectedIndex == i;
    timeOptions[i]->ev = new ClickEvent();
    timeOptions[i]->ev->type = 1; //rectangle
    timeOptions[i]->ev->x = timeOptions[i]->x-35;
    timeOptions[i]->ev->y = timeOptions[i]->y;
    timeOptions[i]->ev->m1 = timeOptions[i]->w;
    timeOptions[i]->ev->m2 = timeOptions[i]->h;
    auto f = [i,timeOptions,timeOptionsCount](){      
      DrawCtrl::selectRadio(i,timeOptions,timeOptionsCount);
    };
    timeOptions[i]->ev->onClickCallback = new LambdaCallback<decltype(f)>(f);
    EvtCtrl::addScreenEvent(timeOptions[i]->ev);
  }
  timeOptions[0]->text = "7 min";
  timeOptions[1]->text = "6 min";
  timeOptions[2]->text = "5 min";
  timeOptions[3]->text = "4 min";
  timeOptions[4]->text = "3 min";
  timeOptions[5]->text = "2 min";
  timeOptions[6]->text = "1 min";
}

static void ScreenTime::initTime() {
  for(uint8_t i = 0; i < optionsCount; i++) {   
    if (options[i]->selected) {
      ScreenTimeGates::currentLevel = i+1;
      break;
    }
  }

  for(uint8_t i = 0; i < timeOptionsCount; i++) {   
    if (timeOptions[i]->selected) {
      ScreenTimeGates::currentTime = (timeOptionsCount-i) * 60l * 1000;
      break;
    }
  }

  ScreensCtrl::goTo(40,nullptr,true);
}

static void ScreenTime::draw(TextInfo titleInfo, char* params[]) {  
  initRadios(titleInfo);
  TSCtrl::tft.setCursor(options[0]->x-35,titleInfo.y+titleInfo.h+5);
  TSCtrl::tft.setTextSize(2);
  TSCtrl::tft.setTextColor(DEFAULT_TEXT_COLOR);
  TSCtrl::tft.print(F("Iniciar no nivel:"));
  TSCtrl::tft.setCursor(timeOptions[0]->x-35,titleInfo.y+titleInfo.h+5);
  TSCtrl::tft.print(F("Tempo:"));
  for(uint8_t i = 0; i < optionsCount; i++) {   
    DrawCtrl::drawRadio(options[i]);
  }
  for(uint8_t i = 0; i < timeOptionsCount; i++) {   
    DrawCtrl::drawRadio(timeOptions[i]);
  }

  EvtCtrl::addScreenEvent(DrawCtrl::drawClickable(
    []{
      ScreenTime::initTime();
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