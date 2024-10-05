#ifndef TSCTRL_H
#define TSCTRL_H

#include <MCUFRIEND_kbv.h>  // Biblioteca MCUFRIEND_kbv
#include <TouchScreen.h>   // Biblioteca para touchscreen
#include "LambdaCallback.h"
#include "Utils.h"




struct TSCtrl {

  // Definição dos pinos do touchscreen
  static const int TS_XP  = 8;  // Substitua pelo pino correto
  static const int TS_YP  = A3;//17;//A3;  // Substitua pelo pino correto
  static const int TS_XM  = A2;//16;//A2;  // Substitua pelo pino correto
  static const int TS_YM  = 9;  // Substitua pelo pino correto

  // Definição das variáveis de calibração do touchscreen
  static const int TS_MINX  = 80;
  static const int TS_MAXX  = 960;
  static const int TS_MINY  = 100;
  static const int TS_MAXY  = 930;

  // Configuração do touchscreen
  static TouchScreen ts;

  static MCUFRIEND_kbv tft;

  static void init();
  

};

#endif // TSCTRL_H