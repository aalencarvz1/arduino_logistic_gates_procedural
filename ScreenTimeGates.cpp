#include "ScreenTimeGates.h"
#include "Gates.h"
#include "DrawCtrl.h"
#include "EvtCtrl.h"
#include "Colors.h"
#include <stdint.h>
#include <Arduino.h>


//STATIC INITIALIZATIONS
static uint8_t ScreenTimeGates::levelCount = 7;
static ClickEvent* ScreenTimeGates::confirmEvent = nullptr;
static bool ScreenTimeGates::nextEnabled = false;
static ClickEvent* ScreenTimeGates::nextEv = nullptr;
static uint8_t ScreenTimeGates::currentLevel = 1;
static uint8_t ScreenTimeGates::currentPhase = 1;
static uint8_t ScreenTimeGates::currentGateIndex = 255;
static uint8_t ScreenTimeGates::currentPontuation = 0;
static double ScreenTimeGates::gateX = 0;
static double ScreenTimeGates::gateY = 0;
static double ScreenTimeGates::gateSize = 0;
static double ScreenTimeGates::gateWidth = 0;
static char* ScreenTimeGates::currentGateName = GATES_NAMES[0];
static Gate* ScreenTimeGates::currentGate = nullptr;
static Circuit* ScreenTimeGates::currentCircuit = nullptr;
static TextInfo ScreenTimeGates::titleInfo;
static TextInfo ScreenTimeGates::subTitleInfo;
static bool ScreenTimeGates::initialState = false;
static bool ScreenTimeGates::updateInitialState = false;

static unsigned long ScreenTimeGates::previousMillis = 0;
static unsigned long ScreenTimeGates::initialTime = 0;
static unsigned long ScreenTimeGates::currentTime = 0;
static unsigned long ScreenTimeGates::interval = 1000;


static void ScreenTimeGates::freeMemory() {
  currentPontuation = 0;
  currentGateName = GATES_NAMES[0];
  if (currentGate != nullptr) {
    delete currentGate;
    currentGate = nullptr;
  }
  if (currentCircuit != nullptr) {
    delete currentCircuit;
    currentCircuit = nullptr;
  }
  confirmEvent = nullptr;
  nextEnabled = false;
  nextEv = nullptr;
  //currentLevel = 1;
  currentPhase = 1;
  currentPontuation = 0;
  gateX = 0;
  gateY = 0;
  gateSize = 0;
  gateWidth = 0;
  currentGateName = GATES_NAMES[0];
  currentGate = nullptr;
  currentCircuit = nullptr;
  initialState = false;
  updateInitialState = false;
  if (EvtCtrl::onTime != nullptr) {
    delete EvtCtrl::onTime;
    EvtCtrl::onTime = nullptr;
  }
  previousMillis = millis();
  //currentTime = 10000;
  interval = 1000;
}

static void ScreenTimeGates::drawConfirmButton(){
  confirmEvent = DrawCtrl::drawClickable(
    []{
      ScreenTimeGates::confirm();
    },
    nullptr,
    1,
    TSCtrl::tft.width()-DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN-120,
    0,
    120,
    25,
    0,
    3,
    Colors::GREEN,
    true,
    "Confirmar"
  );
  EvtCtrl::addScreenEvent(confirmEvent);
}

static void ScreenTimeGates::drawTime(){
  //Serial.println(F("INIT ScreenPoints1::drawTime"));
  Serial.println(String(millis())+","+String(previousMillis)+","+String(currentTime));
  if (millis() - previousMillis >= interval) {    

    currentTime = currentTime - interval;
    previousMillis = millis();  

    TSCtrl::tft.fillRect(TSCtrl::tft.width() - DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN - 80, titleInfo.y + titleInfo.h + DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + 17,79,19,DEFAULT_BACKGROUND_COLOR);
    char timeString[5]; // Array de char para armazenar o resultado
    uint8_t minutes = (currentTime / 1000) / 60;
    uint8_t seconds = (currentTime / 1000) % 60; 
    
    sprintf(timeString, "%02d:%02d", minutes, seconds);
    DrawCtrl::drawCenteredText(timeString,titleInfo.y + titleInfo.h + DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + 25,TSCtrl::tft.width() - DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN - 80 / 2,2,TFT_CYAN);
    if (minutes <= 0 && seconds <= 0) {
      delete EvtCtrl::onTime;
      EvtCtrl::onTime = nullptr;
      drawEndGame();
    }
  }
  
  //Serial.println(F("END ScreenPoints1::drawTime"));
}

static void ScreenTimeGates::drawLevelAndPhase() {
  TSCtrl::tft.fillRect(
    DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+5 + 70, 
    titleInfo.y + titleInfo.h+5,
    20,
    38,
    DEFAULT_BACKGROUND_COLOR
  );
  TSCtrl::tft.setCursor(DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+5 + 70,titleInfo.y + titleInfo.h+5);
  TSCtrl::tft.setTextSize(2);
  TSCtrl::tft.setTextColor(TFT_CYAN);
  TSCtrl::tft.print(currentLevel);

  TSCtrl::tft.setCursor(DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+5 + 70,titleInfo.y + titleInfo.h+25);
  TSCtrl::tft.setTextSize(2);
  TSCtrl::tft.setTextColor(TFT_DARKGREY);
  TSCtrl::tft.print(currentPhase);
}

static void ScreenTimeGates::clearGateSapce(){
  //clear gate space
  double gateSize = TSCtrl::tft.height() * 0.4;
  TSCtrl::tft.fillRect(
    gateX - DEFAULT_GATE_MAX_INPUT_BUTTON_RADIUS - 5,
    gateY - gateSize - gateSize * DEFAULT_GATE_INPUT_CONNECTOR_SIZE_PERC - DEFAULT_GATE_MAX_INPUT_BUTTON_RADIUS - 5,
    gateWidth + DEFAULT_GATE_MAX_INPUT_BUTTON_RADIUS * 2 + 10,
    gateSize + (gateSize * DEFAULT_GATE_INPUT_CONNECTOR_SIZE_PERC) * 2 + DEFAULT_GATE_MAX_INPUT_BUTTON_RADIUS * 2 + 10,
    DEFAULT_BACKGROUND_COLOR
  );
}

static void ScreenTimeGates::clearAllSpaces(){
  //clear spaces
  TSCtrl::tft.fillRoundRect(
    DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH,
    DEFAULT_WINDOW_CONTAINER_AFTER_SUBTITLE_Y+20,
    TSCtrl::tft.width()-DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN*2-DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH*2,
    TSCtrl::tft.height()-DEFAULT_WINDOW_CONTAINER_AFTER_SUBTITLE_Y-21,
    DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_RADIUS,
    DEFAULT_BACKGROUND_COLOR    
  );
  TSCtrl::tft.fillRect(
    DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH+95,
    DEFAULT_WINDOW_CONTAINER_AFTER_SUBTITLE_Y,
    TSCtrl::tft.width()-DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN*2-DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH*2-95*2,
    DEFAULT_WINDOW_CONTAINER_SUBTITLE_H,
    DEFAULT_BACKGROUND_COLOR
  );
}

static void ScreenTimeGates::drawGateLevel(bool pClearGateSpace) {
  if (pClearGateSpace) {
    clearGateSapce();
  }
  if (currentLevel <= 4) { 
    uint8_t connectorCount = 2;
    gateSize = TSCtrl::tft.height() * 0.4;
    gateWidth = gateSize;    
    gateY = TSCtrl::tft.height() / 2 + gateSize/2 + titleInfo.h;    
    if (currentLevel == 1) {
      currentGateIndex = getRandomic(0,3,currentGateIndex);
    } else if (currentLevel == 2) {
      currentGateIndex = getRandomic(3,GATES_NAMES_COUNT,currentGateIndex);
    } else if (currentLevel > 2) {
      currentGateIndex = 2;
      while (currentGateIndex == 2) { //2 = not
        currentGateIndex = getRandomic(0,GATES_NAMES_COUNT,currentGateIndex);
      }     
    }     
    if (currentLevel == 3) {
      connectorCount = getRandomic(3,5);
      gateWidth = gateWidth * 1.3;

      TSCtrl::tft.fillRect(subTitleInfo.x-10,subTitleInfo.y-8,subTitleInfo.w+10,subTitleInfo.h+4, DEFAULT_BACKGROUND_COLOR);
      subTitleInfo = DrawCtrl::drawCenteredText("Mude o estado inicial",titleInfo.y + titleInfo.h +10);
      updateInitialState = true;
    } else if (currentLevel > 3) {      
      TSCtrl::tft.fillRect(subTitleInfo.x-10,subTitleInfo.y-8,subTitleInfo.w+10,subTitleInfo.h+4, DEFAULT_BACKGROUND_COLOR);
      subTitleInfo = DrawCtrl::drawCenteredText("Mude o estado inicial",titleInfo.y + titleInfo.h +10);
      updateInitialState = true;
      connectorCount = getRandomic(4,7);
      gateWidth = gateWidth * 1.5;
    } else {
      subTitleInfo = DrawCtrl::drawCenteredText("Ative a porta",titleInfo.y + titleInfo.h +10);
      updateInitialState = false;
    }

    gateX = TSCtrl::tft.width()/2-gateWidth/2;

    
    currentGate = new Gate(
      currentGateIndex,
      gateX,
      gateY,
      gateSize,
      gateWidth,
      connectorCount
    );
    initGateMeasurements(currentGate);
    setBit(currentGate->packedFlags,7,false);//7-visible output
    DrawCtrl::drawGate(currentGate);
    if (currentLevel < 3) {
      DrawCtrl::drawCenteredText(GATES_NAMES[currentGateIndex],currentGate->y-currentGate->h/2);
    }
    initialState = getBit(currentGate->packedInputs,7); //7-outputState
  }
}

static void ScreenTimeGates::drawCircuitLevel(bool pClearSpaces) {
  clearAllSpaces();
  uint8_t* gatesIds = nullptr;  
  switch (currentLevel) {
    case 5:
      updateInitialState = false;
      switch (currentPhase) {
        case 1:
          gatesIds = new uint8_t[4]{
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT),
            7,
            255
          }; 
          TSCtrl::tft.fillRect(subTitleInfo.x-10,subTitleInfo.y-8,subTitleInfo.w+10,subTitleInfo.h+4, DEFAULT_BACKGROUND_COLOR);
          subTitleInfo = DrawCtrl::drawCenteredText("Ative o circuito",titleInfo.y + titleInfo.h +10);            
          currentCircuit = new Circuit(2,gatesIds);
          break;
        case 2:
          gatesIds = new uint8_t[3]{
            2,
            getRandomic(0,GATES_NAMES_COUNT),
            255
          }; 
          currentCircuit = new Circuit(2,gatesIds);
          break;
        case 3:
        case 4:
        case 5:
          gatesIds = new uint8_t[4]{
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            255
          }; 
          currentCircuit = new Circuit(2,gatesIds);
          break;
      }
      break;
    case 6:      
      updateInitialState = true;
      Gate *g,*g0,*g1,*g2;
      switch (currentPhase) {
        case 1: 
        case 2: 
        case 4: 
          TSCtrl::tft.fillRect(subTitleInfo.x-10,subTitleInfo.y-8,subTitleInfo.w+10,subTitleInfo.h+4, DEFAULT_BACKGROUND_COLOR);
          subTitleInfo = DrawCtrl::drawCenteredText("Mude o estado inicial",titleInfo.y + titleInfo.h +10);
          gatesIds = new uint8_t[5]{
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            7,
            255
          }; 
          currentCircuit = new Circuit(3,gatesIds);
          g0 = currentCircuit->gates[currentCircuit->gateCount-1];
          g1 = currentCircuit->gates[currentCircuit->gateCount-2];
          g2 = currentCircuit->gates[currentCircuit->gateCount-3];
          g = currentCircuit->createGate(
            7,
            false,
            g2->x + g2->w + (g1->x - (g2->x + g2->w)) / 2 - g0->w / 2,
            0,
            0,
            0,
            g2,
            1
          );
          addConnectedGate(g,g1,0);
          DrawCtrl::drawGate(g);
          currentCircuit->createGate(7,true,0,0,0,0,g1,1);
          break;        
        case 3: 
        case 5: 
          gatesIds = new uint8_t[4]{
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            255
          }; 
          currentCircuit = new Circuit(2,gatesIds);
          break;        
      }
      break;
    case 7:      
      updateInitialState = true;
      switch (currentPhase) {
        case 1: 
          TSCtrl::tft.fillRect(subTitleInfo.x-10,subTitleInfo.y-8,subTitleInfo.w+10,subTitleInfo.h+4, DEFAULT_BACKGROUND_COLOR);
          subTitleInfo = DrawCtrl::drawCenteredText("Mude o estado inicial",titleInfo.y + titleInfo.h +10);
          gatesIds = new uint8_t[14]{
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            2,
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            2,
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            255
          }; 
          currentCircuit = new Circuit(4,gatesIds);
          break;
        case 2: 
          gatesIds = new uint8_t[13]{
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            2,
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            255
          }; 
          currentCircuit = new Circuit(4,gatesIds);
          break;
        case 3: 
          gatesIds = new uint8_t[12]{
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            2,
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            2,
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            255
          }; 
          currentCircuit = new Circuit(4,gatesIds);
          break;
        case 4: 
        case 5: 
          gatesIds = new uint8_t[16]{
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT,2),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            getRandomic(0,GATES_NAMES_COUNT),
            255
          }; 
          currentCircuit = new Circuit(4,gatesIds);
          break;
      }
      break;
    default:
      drawEndGame();
      break;
  }
  if (gatesIds != nullptr) {
    delete[] gatesIds;
    gatesIds = nullptr;
  }

  if (currentCircuit != nullptr && currentCircuit->gates != nullptr) {
    initialState = getBit(currentCircuit->gates[0]->packedInputs,7); //7-outputState
  }
}

static void ScreenTimeGates::drawNextPhase(bool pClearSpaces){
  nextEnabled = false;
  drawNavigationButtons();
  EvtCtrl::clearTransitoryEvents();
  if (currentGate != nullptr) {
    delete currentGate;
    currentGate = nullptr;
  }
  if (currentCircuit != nullptr) {
    delete currentCircuit;
    currentCircuit = nullptr;
  }
  drawLevelAndPhase();  
  if (currentLevel <= 4) { //gates
    drawGateLevel(pClearSpaces);
  } else { //circuits  
    drawCircuitLevel(pClearSpaces);
  }
  confirmEvent->enabled = true;
}



static void ScreenTimeGates::incPhase(){
  if (currentPhase < 5) {
    currentPhase++;
  } else {
    currentLevel ++;
    currentPhase = 1;
  }
}

static void ScreenTimeGates::drawResult(bool result, bool clearOnly) {
  uint8_t halfSize = 18;
  uint16_t centerX = TSCtrl::tft.width() - DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN * 2 - halfSize - 17;
  uint16_t centerY = titleInfo.y + titleInfo.h + 62;
  TSCtrl::tft.fillRect(centerX - halfSize - 2, centerY - halfSize - 2, halfSize*2 + 4, halfSize*2 + 4, DEFAULT_BACKGROUND_COLOR);
  if (!clearOnly) {
    if (result) {
      // Desenha o símbolo de check verde
      TSCtrl::tft.drawLine(centerX - halfSize, centerY, centerX - halfSize / 2, centerY + halfSize, TFT_GREEN);
      TSCtrl::tft.drawLine(centerX - halfSize / 2, centerY + halfSize, centerX + halfSize, centerY - halfSize, TFT_GREEN);
    } else {
      // Desenha o símbolo de X vermelho
      TSCtrl::tft.drawLine(centerX - halfSize, centerY - halfSize, centerX + halfSize, centerY + halfSize, TFT_RED);
      TSCtrl::tft.drawLine(centerX + halfSize, centerY - halfSize, centerX - halfSize, centerY + halfSize, TFT_RED);
    }
  }
}

static void ScreenTimeGates::confirm(){
  bool result = false;
  confirmEvent->enabled = false;
  if (currentGate != nullptr) {
    bool outputState = getBit(currentGate->packedInputs,7); 
    if ((updateInitialState && outputState != initialState) || (
      !updateInitialState && outputState
    )) {
      result = true;
      currentPontuation++;
      setBit(currentGate->packedFlags,7,true);//7-visible output;
      DrawCtrl::drawGateOutputButton(currentGate);
    }            
  } else if (currentCircuit != nullptr) {
    bool outputState = getBit(currentCircuit->gates[0]->packedInputs,7); 
    if ((updateInitialState && outputState != initialState) || (
      !updateInitialState && outputState
    )) {
      result = true;
      currentPontuation++;
      for(uint8_t i = 0; i < currentCircuit->gateCount; i++) {
        setBit(currentCircuit->gates[i]->packedFlags,7,true);//7-visible output;
        DrawCtrl::drawGateOutputButton(currentCircuit->gates[i]);
      }
    }
  }
  drawResult(result);
  nextEnabled = result;
  confirmEvent->enabled = !result;
  drawNavigationButtons();
}

static void ScreenTimeGates::drawNavigationButtons(){
  uint16_t color = TFT_YELLOW;
  void* onClick = nullptr;

  if (nextEnabled) {
    if (nextEv == nullptr) {
      onClick = []{
        ScreenTimeGates::drawResult(false,true);
        ScreenTimeGates::incPhase();
        ScreenTimeGates::drawNextPhase();
      };
    } 
  } else {
    color = DEFAULT_BACKGROUND_COLOR;
  };
  if (nextEnabled && onClick != nullptr) {
    //Serial.println("creating next event");
    nextEv = DrawCtrl::drawClickable(
      onClick,
      nullptr,
      3,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 55,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2-20,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 55,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2+20,
      0,
      5,
      color
    );    
    EvtCtrl::addScreenEvent(nextEv);
  } else if (nextEnabled) {
    //Serial.println("drawing prev button");
    double x2=0;
    double y2=0;
    double r1=0;
    DrawCtrl::drawComponent(
      3,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 55,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2-20,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 55,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2+20,
      x2,
      y2,
      r1,
      3,
      color
    );
  } else {
    TSCtrl::tft.fillRect(
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+ + TSCtrl::tft.width() - 60,
      DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + TSCtrl::tft.height() /2-25,
      45,
      45,
      color
    );
  };
  if (nextEv != nullptr) {
    nextEv->enabled = nextEnabled;
  }
}

static void ScreenTimeGates::draw(TextInfo pTitleInfo, char* params[]) {
  freeMemory();
  titleInfo = pTitleInfo;
  if (params != nullptr && params[0] != nullptr) {
    currentGateName = params[0];
  } else {
    currentGateName = GATES_NAMES[0];
  }
  currentGateName = getPrevGateName(currentGateName);
  drawConfirmButton();

  TSCtrl::tft.setCursor(DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+5,titleInfo.y + titleInfo.h+5);
  TSCtrl::tft.setTextSize(2);
  TSCtrl::tft.setTextColor(TFT_CYAN);
  TSCtrl::tft.print("Nivel");

  TSCtrl::tft.setCursor(DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+5,titleInfo.y + titleInfo.h+25);
  TSCtrl::tft.setTextSize(2);
  TSCtrl::tft.setTextColor(TFT_DARKGREY);
  TSCtrl::tft.print("Fase");

  TSCtrl::tft.setCursor(TSCtrl::tft.width() - DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN - 71, titleInfo.y + titleInfo.h + 5);
  TSCtrl::tft.setTextColor(TFT_CYAN);
  TSCtrl::tft.print("Tempo");

  //drawCurrentPontuation();

  auto f = [](){      
    ScreenTimeGates::drawTime();
  };
  if (EvtCtrl::onTime != nullptr) {
    delete EvtCtrl::onTime;
  }
  EvtCtrl::onTime = new LambdaCallback<decltype(f)>(f);
  initialTime = currentTime;
  previousMillis = millis();  

  drawNextPhase(false);
}

static void ScreenTimeGates::drawEndGame() {
  if (EvtCtrl::onTime != nullptr) {
    delete EvtCtrl::onTime;
    EvtCtrl::onTime = nullptr;
  }
  TSCtrl::tft.fillRect(
    TSCtrl::tft.width()-DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN-121,
    0,
    121,
    26,
    DEFAULT_BACKGROUND_COLOR
  );

  TSCtrl::tft.fillRoundRect(
    DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH,
    DEFAULT_WINDOW_CONTAINER_Y,
    TSCtrl::tft.width()-DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN*2-DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH*2,
    DEFAULT_WINDOW_CONTAINER_H,
    DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_RADIUS,
    DEFAULT_BACKGROUND_COLOR
  );

  DrawCtrl::drawCenteredText("Seu tempo foi",DEFAULT_WINDOW_CONTAINER_Y+35);

  TSCtrl::tft.fillCircle(TSCtrl::tft.width() / 2, TSCtrl::tft.height() / 2 + 20, TSCtrl::tft.height() * 0.2, TFT_RED);
  TSCtrl::tft.drawCircle(TSCtrl::tft.width() / 2, TSCtrl::tft.height() / 2 + 20, TSCtrl::tft.height() * 0.25, TFT_RED);
  TSCtrl::tft.setCursor(TSCtrl::tft.width() / 2-58, TSCtrl::tft.height() / 2 + 2);
  TSCtrl::tft.setTextSize(4);

  currentTime = initialTime - currentTime;

  uint8_t minutes = (currentTime / 1000) / 60;
  uint8_t seconds = (currentTime / 1000) % 60; 

  char timeString[5];
    
  sprintf(timeString, "%02d:%02d", minutes, seconds);

  TSCtrl::tft.setTextColor(DEFAULT_TEXT_COLOR);
  Serial.println("current Pontuation: "+String(timeString));
  TSCtrl::tft.print(timeString);
  
}