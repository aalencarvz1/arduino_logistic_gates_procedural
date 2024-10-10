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


static void ScreenPoints::drawRadio(Radio* option) {
  TSCtrl::tft.drawCircle(option->x-20,option->y+7, 7,DEFAULT_TEXT_COLOR);  
  TSCtrl::tft.setCursor(option->x,option->y);
  TSCtrl::tft.setTextColor(DEFAULT_TEXT_COLOR);
  TSCtrl::tft.print(option->text);
  if (option->selected) {
    TSCtrl::tft.drawRoundRect(option->x-35,option->y-5, 300,25,3,TFT_GREEN);
    TSCtrl::tft.fillCircle(option->x-20,option->y+7, 5,TFT_GREEN);
  } else {
    TSCtrl::tft.drawRoundRect(option->x-35,option->y-5, 300,25,3,TFT_DARKGREY);
    TSCtrl::tft.fillCircle(option->x-20,option->y+7, 5,DEFAULT_BACKGROUND_COLOR);
  }
}

static void ScreenPoints::selectOption(uint8_t optionIndex) {  
  int indexPreviousSelected = -1;
  for(uint8_t i = 0; i < optionsCount; i++) {
    if (options[i]->selected) {
      indexPreviousSelected = i;
    }
    options[i]->selected = false;  
  }
  if (indexPreviousSelected > -1 && indexPreviousSelected != optionIndex) {
    drawRadio(options[indexPreviousSelected]);
  }
  options[optionIndex]->selected = true;
  if (indexPreviousSelected != optionIndex) {  
    drawRadio(options[optionIndex]);
  }
} 

static void ScreenPoints::initRadios(TextInfo titleInfo) {
  freeMemory();
  options = new Radio*[optionsCount]{nullptr};
  for(int i = 0; i < optionsCount; i++) {
    options[i] = new Radio();
    options[i]->x = DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + 45;
    options[i]->y = titleInfo.y+titleInfo.h+45+i*30;
    options[i]->selected = currentSelectedIndex == i;
    options[i]->ev = new ClickEvent();
    options[i]->ev->type = 1; //rectangle
    options[i]->ev->x = options[i]->x;
    options[i]->ev->y = options[i]->y;
    options[i]->ev->m1 = 300;
    options[i]->ev->m2 = 25;
    auto f = [i](){      
      ScreenPoints::selectOption(i);
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
    drawRadio(options[i]);
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