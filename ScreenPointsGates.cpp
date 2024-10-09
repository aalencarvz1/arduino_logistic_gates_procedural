#include "ScreenPointsGates.h"
#include "Gates.h"
#include "DrawCtrl.h"
#include "EvtCtrl.h"
#include "Colors.h"


//STATIC INITIALIZATIONS

static ClickEvent* ScreenPointsGates::confirmEvent = nullptr;
static bool ScreenPointsGates::nextEnabled = false;
static ClickEvent* ScreenPointsGates::nextEv = nullptr;
static uint8_t ScreenPointsGates::currentLevel = 1;
static uint8_t ScreenPointsGates::currentPhase = 1;
static uint8_t ScreenPointsGates::currentPontuation = 0;
static double ScreenPointsGates::gateX = 0;
static double ScreenPointsGates::gateY = 0;
static double ScreenPointsGates::gateSize = 0;
static double ScreenPointsGates::gateWidth = 0;
static char* ScreenPointsGates::currentGateName = GATES_NAMES[0];
static Gate* ScreenPointsGates::currentGate = nullptr;
static Circuit* ScreenPointsGates::currentCircuit = nullptr;
static TextInfo ScreenPointsGates::titleInfo;
static TextInfo ScreenPointsGates::subTitleInfo;
static bool ScreenPointsGates::initialState = false;
static bool ScreenPointsGates::updateInitialState = false;


static void ScreenPointsGates::freeMemory() {
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
}

static void ScreenPointsGates::drawConfirmButton(){
  confirmEvent = DrawCtrl::drawClickable(
    []{
      ScreenPointsGates::confirm();
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

static void ScreenPointsGates::drawCurrentPontuation(){
  //Serial.println(F("INIT ScreenPoints1::drawCurrentPontuation"));
  TSCtrl::tft.fillRect(TSCtrl::tft.width() - DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN - 80, titleInfo.y + titleInfo.h + DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + 17,79,19,DEFAULT_BACKGROUND_COLOR);
  char buffer[10]; // Array de char para armazenar o resultado
  itoa(currentPontuation, buffer, 10);
  DrawCtrl::drawCenteredText(buffer,titleInfo.y + titleInfo.h + DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN + 25,TSCtrl::tft.width() - DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN - 80 / 2,2,TFT_CYAN);
  //Serial.println(F("END ScreenPoints1::drawCurrentPontuation"));
}

static void ScreenPointsGates::drawLevelAndPhase() {
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

static void ScreenPointsGates::clearGateSapce(){
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

static void ScreenPointsGates::clearAllSpaces(){
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

static void ScreenPointsGates::drawGateLevel(bool pClearGateSpace) {
  if (pClearGateSpace) {
    clearGateSapce();
  }
  if (currentLevel <= 4) { 
    uint8_t idGateName = 0;
    uint8_t connectorCount = 2;
    gateSize = TSCtrl::tft.height() * 0.4;
    gateWidth = gateSize;    
    gateY = TSCtrl::tft.height() / 2 + gateSize/2 + titleInfo.h;    
    if (currentLevel == 1) {
      idGateName = random(0,3);
    } else if (currentLevel == 2) {
      idGateName = random(3,GATES_NAMES_COUNT);
    } else if (currentLevel > 2) {
      idGateName = 2;
      while (idGateName == 2) { //2 = not
        idGateName = random(0,GATES_NAMES_COUNT);
      }     
    }     
    if (currentLevel == 3) {
      connectorCount = random(3,5);
      gateWidth = gateWidth * 1.3;
      TSCtrl::tft.fillRect(subTitleInfo.x-10,subTitleInfo.y-8,subTitleInfo.w+10,subTitleInfo.h+4, DEFAULT_BACKGROUND_COLOR);
      subTitleInfo = DrawCtrl::drawCenteredText("Mude o estado inicial",titleInfo.y + titleInfo.h +10);
      updateInitialState = true;
    } else if (currentLevel > 3) {
      connectorCount = random(4,7);
      gateWidth = gateWidth * 1.4;
    } else {
      subTitleInfo = DrawCtrl::drawCenteredText("Ative a porta",titleInfo.y + titleInfo.h +10);
    }

    gateX = TSCtrl::tft.width()/2-gateWidth/2;

    
    currentGate = new Gate(
      idGateName,
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
      DrawCtrl::drawCenteredText(GATES_NAMES[idGateName],currentGate->y-currentGate->h/2);
    }
    initialState = getBit(currentGate->packedInputs,7); //7-outputState
  }
}

static void ScreenPointsGates::drawCircuitLevel(bool pClearSpaces) {
  clearAllSpaces();
  uint8_t* gatesIds = nullptr;  
  switch (currentLevel) {
    case 5:
      updateInitialState = false;
      switch (currentPhase) {
        case 1:
          gatesIds = new uint8_t[4]{0,2,7,255}; 
          TSCtrl::tft.fillRect(subTitleInfo.x-10,subTitleInfo.y-8,subTitleInfo.w+10,subTitleInfo.h+4, DEFAULT_BACKGROUND_COLOR);
          subTitleInfo = DrawCtrl::drawCenteredText("Ative o circuito",titleInfo.y + titleInfo.h +10);            
          currentCircuit = new Circuit(2,gatesIds);
          break;
        case 2:
          gatesIds = new uint8_t[3]{2,0,255}; 
          currentCircuit = new Circuit(2,gatesIds);
          break;
        case 3:
          gatesIds = new uint8_t[4]{1,0,2,255}; 
          currentCircuit = new Circuit(2,gatesIds);
          break;
        case 4:
          gatesIds = new uint8_t[4]{0,0,5,255}; 
          currentCircuit = new Circuit(2,gatesIds);
          break;
        case 5:
          gatesIds = new uint8_t[4]{0,0,1,255}; 
          currentCircuit = new Circuit(2,gatesIds);
          break;
      }
      break;
    case 6:      
      updateInitialState = true;
      Gate *g,*g0,*g1,*g2;
      switch (currentPhase) {
        case 1: 
          TSCtrl::tft.fillRect(subTitleInfo.x-10,subTitleInfo.y-8,subTitleInfo.w+10,subTitleInfo.h+4, DEFAULT_BACKGROUND_COLOR);
          subTitleInfo = DrawCtrl::drawCenteredText("Mude o estado inicial",titleInfo.y + titleInfo.h +10);
          gatesIds = new uint8_t[5]{0,0,5,7,255}; 
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
        case 2: 
          gatesIds = new uint8_t[5]{0,0,1,7,255}; 
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
          gatesIds = new uint8_t[4]{1,3,0,255}; 
          currentCircuit = new Circuit(2,gatesIds);
          break;
        case 4: 
          gatesIds = new uint8_t[5]{1,3,1,7,255}; 
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
        case 5: 
          gatesIds = new uint8_t[4]{6,3,6,255}; 
          currentCircuit = new Circuit(2,gatesIds);
          break;
      }
      break;
    case 7:      
      switch (currentPhase) {
        case 1: 
          TSCtrl::tft.fillRect(subTitleInfo.x-10,subTitleInfo.y-8,subTitleInfo.w+10,subTitleInfo.h+4, DEFAULT_BACKGROUND_COLOR);
          subTitleInfo = DrawCtrl::drawCenteredText("Mude o estado inicial",titleInfo.y + titleInfo.h +10);
          gatesIds = new uint8_t[14]{1,0,0,2,5,4,2,3,2,6,3,1,6,255}; 
          currentCircuit = new Circuit(4,gatesIds);
          break;
        case 2: 
          gatesIds = new uint8_t[13]{6,5,2,3,1,4,5,6,3,6,0,4,255}; 
          currentCircuit = new Circuit(4,gatesIds);
          break;
        case 3: 
          gatesIds = new uint8_t[12]{4,3,2,1,1,2,4,5,6,5,5,255}; 
          currentCircuit = new Circuit(4,gatesIds);
          break;
        case 4: 
          gatesIds = new uint8_t[16]{3,4,5,6,6,5,4,4,5,6,0,4,5,6,4,255}; 
          currentCircuit = new Circuit(4,gatesIds);
          break;
        case 5: 
          gatesIds = new uint8_t[16]{5,5,5,5,4,4,4,4,4,6,6,4,5,6,0,255}; 
          currentCircuit = new Circuit(4,gatesIds);
          break;
      }
      break;
    case 8:      
      switch (currentPhase) {
        case 1: 
          gatesIds = new uint8_t[34]{5,5,5,5,4,4,4,4,4,6,6,4,5,6,0,4,5,5,5,5,4,4,4,4,4,6,6,4,5,6,0,255}; 
          currentCircuit = new Circuit(5,gatesIds);
          break;
      }
  }
  

}

static void ScreenPointsGates::drawNextPhase(bool pClearSpaces){
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



static void ScreenPointsGates::incPhase(){
  if (currentPhase < 5) {
    currentPhase++;
  } else {
    currentLevel ++;
    currentPhase = 1;
  }
}

static void ScreenPointsGates::drawResult(bool result, bool clearOnly) {
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

static void ScreenPointsGates::confirm(){
  bool result = false;
  confirmEvent->enabled = false;
  if (currentGate != nullptr) {
    setBit(currentGate->packedFlags,7,true);//7-visible output;
    DrawCtrl::drawGateOutputButton(currentGate);
    bool outputState = getBit(currentGate->packedInputs,7); 
    if ((updateInitialState && outputState != initialState) || (
      !updateInitialState && outputState
    )) {
      result = true;
      currentPontuation++;
      drawCurrentPontuation();
    }
  } else if (currentCircuit != nullptr) {
    setBit(currentCircuit->gates[0]->packedFlags,7,true);//7-visible output;
    DrawCtrl::drawGateOutputButton(currentCircuit->gates[0]);
    bool outputState = getBit(currentCircuit->gates[0]->packedInputs,7); 
    if ((updateInitialState && outputState != initialState) || (
      !updateInitialState && outputState
    )) {
      result = true;
      currentPontuation++;
      drawCurrentPontuation();
    }
  }
  drawResult(result);
  //delay(DEFAULT_PHASE_TIME_INTERVAL);
  
  //incPhase();
  //drawNextPhase();
  nextEnabled = true;
  drawNavigationButtons();
}

static void ScreenPointsGates::drawNavigationButtons(){
  uint16_t color = TFT_YELLOW;
  void* onClick = nullptr;

  if (nextEnabled) {
    if (nextEv == nullptr) {
      onClick = []{
        ScreenPointsGates::drawResult(false,true);
        ScreenPointsGates::incPhase();
        ScreenPointsGates::drawNextPhase();
      };
    } 
  } else {
    color = DEFAULT_BACKGROUND_COLOR;
  };
  if (nextEnabled && onClick != nullptr) {
    Serial.println("creating next event");
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
    Serial.println("drawing prev button");
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

static void ScreenPointsGates::draw(TextInfo pTitleInfo, char* params[]) {
  freeMemory();
  randomSeed(analogRead(0));
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

  TSCtrl::tft.setCursor(TSCtrl::tft.width() - DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN - 78, titleInfo.y + titleInfo.h + 5);
  TSCtrl::tft.setTextColor(TFT_CYAN);
  TSCtrl::tft.print("Pontos");

  drawCurrentPontuation();

  drawNextPhase(false);
}