#ifndef DRAWCTRL_H
#define DRAWCTRL_H

#include <stdint.h>
#include "Utils.h"
#include "TSCtrl.h"
#include "ClickEvent.h"
#include "Gate.h"


#define DEFAULT_BACKGROUND_COLOR TFT_BLACK
#define DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH 1
#define DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_COLOR TFT_WHITE
#define DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN 5
#define DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_RADIUS 10
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
  static double drawGateNegation(const Gate* g, const double& x, const double&y, const double& h);
  static void drawGateOutputConnector(const Gate* g, const double& x, const double&y, const double& w, const double& h, const double& connectorSize, const double& notRadius);
  static void drawAnd(Gate* g,double x,double y, double h);  
  static void drawOr(Gate* g,double x,double y, double h);  
  static void drawNot(Gate* g,double x,double y, double h);  
  static void drawGate(Gate* g,double x,double y, double h);
};

#endif // DRAWCTRL_H