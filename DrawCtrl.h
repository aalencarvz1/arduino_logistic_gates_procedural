#ifndef DRAWCTRL_H
#define DRAWCTRL_H

#include <stdint.h>
#include "Utils.h"
#include "TSCtrl.h"
#include "ClickEvent.h"
#include "Gate.h"
#include "Radio.h"


#define DEFAULT_BACKGROUND_COLOR TFT_BLACK
#define DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH 1
#define DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_COLOR TFT_WHITE
#define DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN 5
#define DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_RADIUS 10
#define DEFAULT_WINDOW_CONTAINER_Y 27
#define DEFAULT_WINDOW_CONTAINER_H 291
#define DEFAULT_WINDOW_CONTAINER_W 410

#define DEFAULT_WINDOW_CONTAINER_SUBTITLE_H 22
#define DEFAULT_WINDOW_CONTAINER_AFTER_SUBTITLE_Y 49
#define DEFAULT_WINDOW_TITLE_TEXT_SIZE 2
#define DEFAULT_TEXT_SIZE 2
#define DEFAULT_TEXT_COLOR TFT_WHITE
#define DEFAULT_CLICKABLE_COLOR TFT_DARKGREY
#define DEFAULT_CLICKABLE_TEXT_COLOR TFT_WHITE
#define DEFAULT_GATE_INPUT_CONNECTOR_MARGIN_PERC 0.1
#define DEFAULT_GATE_INPUT_CONNECTOR_SIZE_PERC 0.3
#define DEFAULT_GATE_BASE_ARC_HEIGHT_ASPECT_RATIO 0.3
#define DEFAULT_GATE_EXCLUSIVE_SPACE_PERC 0.1
#define DEFAULT_GATE_BASE_OR_ARC_HEIGHT_PERC 0.65
#define DEFAULT_GATE_NOT_RADIUS_PERC 0.12
#define DEFAULT_GATE_MAX_NOT_RADIUS 8.0
#define DEFAULT_GATE_INPUT_RADIUS_PERC 0.15
#define DEFAULT_GATE_INPUT_OFF_COLOR TFT_RED
#define DEFAULT_GATE_INPUT_ON_COLOR TFT_GREEN
#define DEFAULT_GATE_MAX_INPUT_BUTTON_RADIUS 15
#define DEFAULT_GATE_MIN_INPUT_BUTTON_RADIUS 8






//para armazenar informações referente ao dimensionmanto (bounds) de textos em tela
struct TextInfo{
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t w = 0;
  uint16_t h = 0; 
};

struct DrawCtrl{
  static TextInfo drawCenteredText(
    const char* text, 
    const double& y = 0, 
    const double& centerX = TSCtrl::tft.width() * 1.0 / 2.0, 
    const uint8_t& textSize = DEFAULT_TEXT_SIZE, 
    const int& color = DEFAULT_TEXT_COLOR
  );
  static void drawSmoothArc(
    const double& x_center, 
    const double& y_center, 
    const double& radius, 
    const double& start_angle, 
    const double& end_angle, 
    const int& color = TFT_WHITE
  );
  static CircleInfo drawArcFromArrow(
    const double& x1, 
    const double& y1, 
    const double& x2, 
    const double& y2, 
    const double& arcHeight, 
    const int& color = TFT_WHITE
  );
  static void drawRoundedPlay(
    const double& cx,
    const double& cy,
    const double& r,
    const double& round = 0,
    const int& backgroundColor = TFT_BLACK,
    const int& color = TFT_WHITE,
    const double& multiplier = 1
  );

  static void DrawCtrl::drawComponent(
    uint8_t type,
    int16_t x,
    int16_t y,
    int16_t m1,
    int16_t m2,
    double& x2,
    double& y2,
    double& r1,
    double r2=0,
    uint16_t color = DEFAULT_CLICKABLE_COLOR,
    bool filled = true,
    char* text = nullptr,
    uint16_t textColor = DEFAULT_CLICKABLE_TEXT_COLOR
  );

  static ClickEvent* drawClickable(
    void (*staticOnClick)() = nullptr,
    ICallback* onClickCallback = nullptr,  // Ponteiro para o callback
    uint8_t type = 1, //[rect,circle,triangle]
    int16_t x=0,
    int16_t y=0,
    int16_t m1=0,
    int16_t m2=0,
    double r1=0,
    double r2=0,
    uint16_t color = DEFAULT_CLICKABLE_COLOR,
    bool filled = true,
    char* text = nullptr,
    uint16_t textColor = DEFAULT_CLICKABLE_TEXT_COLOR
  );

  static void drawOption(
    const double& pX,
    const double& pY, 
    const double& pR, 
    const int& pColor = TFT_WHITE, 
    const bool& hasPlay = true, 
    const char* pText = nullptr, 
    const void (*pStaticOnClick)() = nullptr, 
    const ICallback* pOnClick = nullptr
  );
  static void drawGateNegation(const Gate* g);
  static void drawGateOutputConnector(const Gate* g);
  static void drawGateInputsButtons(Gate* g);
  static void drawGateOutputButton(Gate* g);
  static void drawAnd(Gate* g);  
  static void drawOr(Gate* g);  
  static void drawNot(Gate* g);  
  static void drawBypass(Gate* g); 
  static void drawGate(Gate* g);
  static void drawRadio(Radio* option);
  static void selectRadio(uint8_t pSelectedIndex, Radio** options, uint8_t optionsCount);
};

#endif // DRAWCTRL_H