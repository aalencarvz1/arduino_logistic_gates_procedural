#include "ScreensCtrl.h"
#include <MemoryUsage.h>
#include "TSCtrl.h"
#include "EvtCtrl.h"
#include "Colors.h"
#include "Gates.h"
#include "Gate.h"
#include "ScreenTutorialGates.h"
#include "ScreenPointsGates.h"
#include "ScreenPoints.h"
#include "ScreenTimeGates.h"
#include "ScreenTime.h"
#include "ScreenMake.h"


//STATIC INITIALIZATIONS
StackArray <uint8_t> ScreensCtrl::stack;


void ScreensCtrl::drawGateButton(
  double pX,
  double pY,
  double pR,
  int col,
  int lin,
  double hSupSpace,
  int pColor,
  const char* pGateName
) {
  auto f = [pGateName](){
    char* params[] = {pGateName, nullptr};
    ScreensCtrl::goTo(20,params);
  };
  pX = pX + (col * (pX*2.0));
  pY = pY + hSupSpace + (lin * (pY * 2.0));
  DrawCtrl::drawOption(
    pX, 
    pY, 
    pR,
    pColor,
    false,   
    pGateName,
    nullptr,
    new LambdaCallback<decltype(f)>(f)
  );

  if (pGateName != nullptr) {    
    Gate* g = new Gate(getGateIndex(pGateName),pX-pR/2.0,pY+pR/2.0,pR);
    setBit(g->packedFlags,5,false);//5-hasInputButtons
    setBit(g->packedFlags,6,false);//6-visibleInputs
    setBit(g->packedFlags,7,false);//5-visibleOutput
    initGateMeasurements(g);
    DrawCtrl::drawGate(g);
    delete g;
  };

}

static void ScreensCtrl::drawHomeScreenOptions(TextInfo titleInfo) {
  DrawCtrl::drawCenteredText("Modos de jogo",titleInfo.h+20);
  double _4Div = TSCtrl::tft.width() / 4.0;
  double itemMenuXPos = _4Div / 2.0;
  double itemMenuYPos = TSCtrl::tft.height() / 2.0;
  double itemR = itemMenuXPos * 0.5;  
  DrawCtrl::drawOption(itemMenuXPos, itemMenuYPos, itemR, TFT_YELLOW,true,"Tutorial",[](){    
    ScreensCtrl::goTo(2);
  });
  DrawCtrl::drawOption(
    itemMenuXPos + _4Div , 
    itemMenuYPos, 
    itemR, 
    Colors::BLUE,
    true,
    "Pontuacao",
    [](){    
      ScreensCtrl::goTo(3);
    }
  );
  DrawCtrl::drawOption(itemMenuXPos + _4Div * 2, itemMenuYPos, itemR, Colors::GREEN,true,"Tempo",[](){    
    ScreensCtrl::goTo(4);
  });
  DrawCtrl::drawOption(itemMenuXPos + _4Div * 3, itemMenuYPos, itemR, TFT_RED,true,"Construa",[](){    
    ScreensCtrl::goTo(5);
  });
}

static void ScreensCtrl::drawTutorialScreenOptions(TextInfo titleInfo) {
  //linha 1, 3 colunas
  int cols = 3;
  int lins = 2;
  double pX = TSCtrl::tft.width() / (cols * 1.0);
  pX = pX / 2.0;
  double pY = TSCtrl::tft.height() / (lins * 1.0);
  pY = pY / 2.0;
  double pR = pX;
  if (pY < pX) {
    pR = pY;
  }
  pR = pR * 0.457;
  int col = 0;
  int lin = 0;
  double marginSup = 15;
  double hSpace = 0;

  drawGateButton(pX,pY,pR,col,lin,marginSup,TFT_DARKCYAN,GATES_NAMES[0]);
  drawGateButton(pX,pY,pR,col+1,lin,marginSup,TFT_DARKGREY,GATES_NAMES[1]);
  drawGateButton(pX,pY,pR,col+2,lin,marginSup,TFT_RED,GATES_NAMES[2]);

  //linha 2, 4 colunas
  cols = 4;
  pX = TSCtrl::tft.width() / cols;
  pX = pX / 2.0;
  col = 0;
  lin++;
  drawGateButton(pX,pY,pR,col,lin,hSpace,TFT_OLIVE,GATES_NAMES[3]);
  drawGateButton(pX,pY,pR,col+1,lin,hSpace,TFT_ORANGE,GATES_NAMES[4]);
  drawGateButton(pX,pY,pR,col+2,lin,hSpace,TFT_MAGENTA,GATES_NAMES[5]);
  drawGateButton(pX,pY,pR,col+3,lin,hSpace,TFT_PURPLE,GATES_NAMES[6]);
}

static void ScreensCtrl::goTo(uint8_t screenId, char* params[], bool popCurrent) {
  //Serial.println(F("INIT ScreensCtrl::goTo"));

  if (!stack.isEmpty()) {
    destroy(stack.peek());
  } 



  //Serial.println("screenId "+String(screenId));
  EvtCtrl::clearAllEvents();
  TSCtrl::tft.fillScreen(DEFAULT_BACKGROUND_COLOR);
  char* title = nullptr;
  bool hasBack = true;
  TextInfo titleInfo;
  switch(screenId) {
    case 1: //home
      title = "PORTAS LOGICAS E CIRCUITOS";
      hasBack = false;
    case 2: //tutorial
      title = "MODO TUTORIAL";
      break;
    case 20: //tutorial gates
      title = "MODO TUTORIAL-PORTAS";
      break;
    case 3: //poins gates
      title = "MODO PONTUACAO";
      break;
    case 30: //poins gates
      title = "MODO PONTUACAO";
      break;
    case 4: //time gates
      title = "MODO TEMPO";
      break;
    case 40: //time gates
      title = "MODO TEMPO";
      break;
    case 5: //time gates
      title = "MODO CONSTRUCAO";
      break;
    default: 
      title = "NAO ENCONTRADO";      
  }

  //screen title
  if (title != nullptr) {
    titleInfo = DrawCtrl::drawCenteredText(title,DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+8,TSCtrl::tft.width()/2,DEFAULT_WINDOW_TITLE_TEXT_SIZE,DEFAULT_TEXT_COLOR);
  }

  //back button
  if (hasBack) {    
    ClickEvent* ev = DrawCtrl::drawClickable(
      [](){
        ScreensCtrl::goBack();    
      },
      nullptr,
      1,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN,
      30,
      20,
      0,
      3,
      TFT_DARKGREY,
      true,
      "<"
    );  
    EvtCtrl::addScreenEvent(ev);  
  }

  //screen container
  TSCtrl::tft.drawRoundRect(
    DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN,
    titleInfo.h+DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+5,
    TSCtrl::tft.width() - DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN * 2,
    TSCtrl::tft.height() - (titleInfo.h+DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN*2),
    DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_RADIUS,
    DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_COLOR
  );


  //screen 1 options 
  switch(screenId) {
    case 1: //home
      drawHomeScreenOptions(titleInfo);      
      break;
    case 2: //tutorial
      drawTutorialScreenOptions(titleInfo);      
      break;
    case 20: //tutorial-gates
      ScreenTutorialGates::draw(titleInfo, params);
      break;
    case 3: //points
      ScreenPoints::draw(titleInfo);
      break;
    case 4: //time
      ScreenTime::draw(titleInfo);
      break;
    case 30: //points
      ScreenPointsGates::draw(titleInfo, params);
      break;
    case 40: //time
      ScreenTimeGates::draw(titleInfo, params);
      break;
    case 5: //time
      ScreenMake::draw(titleInfo, params);
      break;
    default: 
      DrawCtrl::drawCenteredText("Nada aqui",TSCtrl::tft.height()/2);      
  }

  //stack update
  if (!stack.isEmpty()) {
    if (popCurrent) {
      stack.pop();
    }
    if (stack.peek() != screenId) {
      stack.push(screenId);
    }
  } else {
    stack.push(screenId);
  }
  FREERAM_PRINT;
  //Serial.println(F("END ScreensCtrl::goTo"));
}

static void ScreensCtrl::destroy(uint8_t screenId) {
   switch(screenId) {
    case 20:
      ScreenTutorialGates::freeMemory();
      break;
    case 3:
      ScreenPoints::freeMemory();
    case 30:
      ScreenPointsGates::freeMemory();
    case 4:
      ScreenTime::freeMemory();
    case 40:
      ScreenTimeGates::freeMemory();
    case 5:
      ScreenMake::freeMemory();
      break;    
  };
}

//navigate to previous screen on stack
static void ScreensCtrl::goBack() { 
  destroy(stack.pop());
  ScreensCtrl::goTo(stack.peek());
}