#include "TSCtrl.h"
#include <math.h>
#include "ClickEvent.h"


//STATIC INITIALIZATIONS
MCUFRIEND_kbv TSCtrl::tft;
TouchScreen TSCtrl::ts = TouchScreen(TSCtrl::TS_XP, TSCtrl::TS_YP, TSCtrl::TS_XM, TSCtrl::TS_YM, 300);

/**
* TSCtrl
*/
void TSCtrl::init(){
    // Configuração do touchscreen
    // Inicializa o TFT    
    int identifier = tft.readID();
    //Serial.println("TFT SHIELD IDENTIFIER: "+String(identifier));
    if (identifier == 0x9325 || identifier == 0x9341 || identifier == 0x9486 || identifier == 0x7796) {
      tft.begin(identifier);
    } else {
      tft.begin(0x9486);  // Força o uso de um controlador comum, se necessário
    }
    tft.setRotation(1);  // Ajuste a rotação conforme necessário
};




