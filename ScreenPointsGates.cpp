#include "ScreenPointsGates.h"
#include "Gates.h"
#include "DrawCtrl.h"
#include "EvtCtrl.h"
#include "Colors.h"


//STATIC INITIALIZATIONS
static uint8_t ScreenPointsGates::currentLevel = 1;
static uint8_t ScreenPointsGates::currentPhase = 1;
static uint8_t ScreenPointsGates::currentPontuation = 0;
static double ScreenPointsGates::gateX = 0;
static double ScreenPointsGates::gateY = 0;
static double ScreenPointsGates::gateSize = 0;
static double ScreenPointsGates::gateWidth = 0;
static char* ScreenPointsGates::currentGateName = GATES_NAMES[0];
static Gate* ScreenPointsGates::currentGate = nullptr;
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
}

static void ScreenPointsGates::drawConfirmButton(){
  EvtCtrl::addScreenEvent(
    DrawCtrl::drawClickable(
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
    )
  );
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
  
}

static void ScreenPointsGates::drawNextPhase(bool pClearSpaces){
  EvtCtrl::clearTransitoryEvents();
  if (currentGate != nullptr) {
    delete currentGate;
    currentGate = nullptr;
  }
  drawLevelAndPhase();  
  if (currentLevel <= 4) { //gates
    drawGateLevel(pClearSpaces);
  } else { //circuits  
    drawCircuitLevel(pClearSpaces);
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
  }
  drawResult(result);
  delay(DEFAULT_PHASE_TIME_INTERVAL);
  drawResult(false,true);
  incPhase();
  drawNextPhase();
}