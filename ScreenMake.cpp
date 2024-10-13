#include "ScreenMake.h"
#include "Gates.h"
#include "DrawCtrl.h"
#include "TSCtrl.h"
#include "Gate.h"
#include "EvtCtrl.h"

//STATIC INITIALIZATIONS
static TextInfo ScreenMake::titleInfo;
static int ScreenMake::buttonSize = 40;
static Circuit* ScreenMake::currentCircuit = nullptr;

static void ScreenMake::freeMemory() {
  if (currentCircuit != nullptr) {
    delete currentCircuit;
    currentCircuit = nullptr;
  }
  currentCircuit = nullptr;
}


static void ScreenMake::addGate(uint8_t pGateId) {
  uint8_t pOutputInpuIndex = 255;
  Gate* pOutputGate = currentCircuit->getNextGateInputConnector(pOutputInpuIndex);
  uint8_t currentLevel = pOutputGate->currentCircuitLevel + 1;
  if (currentLevel < 4) {
    currentCircuit->createGate(pGateId);
    currentLevel = currentCircuit->gates[currentCircuit->gateCount-1]->currentCircuitLevel + 1;
    Serial.println("currentLevel "+String(currentLevel) + "," + String(currentCircuit->gateLevelCount));
    if (currentLevel > currentCircuit->gateLevelCount) {
      uint8_t* gatesTypes = new uint8_t[currentCircuit->gateCount+1]{255};
      for(uint8_t i = 0; i < currentCircuit->gateCount; i++) {
        gatesTypes[i] = currentCircuit->gates[i]->type;
        Serial.println("gate id " + String(gatesTypes[i]));
      }
      gatesTypes[currentCircuit->gateCount] = 255;
      EvtCtrl::clearTransitoryEvents();
      delete currentCircuit;
      currentCircuit = nullptr;

      TSCtrl::tft.fillRoundRect(
        DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH,
        DEFAULT_WINDOW_CONTAINER_Y,
        TSCtrl::tft.width()-DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN*2-DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH*2-(buttonSize+5) * 2,
        DEFAULT_WINDOW_CONTAINER_H+1,
        DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_RADIUS,
        DEFAULT_BACKGROUND_COLOR
      );

      TSCtrl::tft.fillRect(
        DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH + TSCtrl::tft.width()-DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN*2-DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH*2-(buttonSize+5) * 2,
        DEFAULT_WINDOW_CONTAINER_Y + (buttonSize+5) * 3,
        buttonSize+5,
        buttonSize+5,
        DEFAULT_BACKGROUND_COLOR
      );

      TSCtrl::tft.fillRect(
        DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN+DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH + TSCtrl::tft.width()-DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN*2-DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_WIDTH*2-(buttonSize+5) * 2,
        DEFAULT_WINDOW_CONTAINER_Y + (buttonSize+5) * 4,
        (buttonSize+5) * 2 - DEFAULT_WINDOW_CONTENT_CONTAINER_BORDER_RADIUS,
        DEFAULT_WINDOW_CONTAINER_H - (buttonSize+5) * 4,
        DEFAULT_BACKGROUND_COLOR
      );

      currentCircuit = new Circuit(currentLevel,gatesTypes);
      delete gatesTypes;
      gatesTypes = nullptr;
    }
  }
}

static void ScreenMake::drawGateButton(uint8_t pGateId, int x, int y) {

  auto f = [pGateId](){      
    ScreenMake::addGate(pGateId);
  };

  EvtCtrl::addScreenEvent(DrawCtrl::drawClickable(
    nullptr,
    new LambdaCallback<decltype(f)>(f),
    1,
    x,
    y,
    buttonSize,
    buttonSize,
    0,
    3,
    DEFAULT_TEXT_COLOR,
    false
  ));

  int pGateSize = buttonSize * 0.5;

  Gate* g = new Gate(
    pGateId,
    x+buttonSize/2-pGateSize/2,
    y+buttonSize - buttonSize * 0.25,
    pGateSize,
    pGateSize
  );
  initGateMeasurements(g);

  setBit(g->packedFlags,5,false);
  setBit(g->packedFlags,6,false);
  setBit(g->packedFlags,7,false);

  DrawCtrl::drawGate(g);
  delete g;
  g = nullptr;
}

static void ScreenMake::draw(TextInfo pTitleInfo, char* params[]) {
  titleInfo = pTitleInfo;
  for(uint8_t i = 0; i < GATES_NAMES_COUNT; i++) {
    drawGateButton(
      i,
      TSCtrl::tft.width() - DEFAULT_WINDOW_CONTENT_CONTAINER_MARGIN * 2 - buttonSize * (i > 2 ? 1 : 2.1),
      DEFAULT_WINDOW_CONTAINER_Y + 5 + (i - (i > 2 ? 3 : 0)) * (buttonSize + 5)
    );
  }

  currentCircuit = new Circuit(2);
}
