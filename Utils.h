#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

struct DPoint{
  double x,y;
};

struct UIPoint{
  int x,y;
};

struct CircleInfo {
  double x,y,r;
};

String boolToString(const bool& v);
void toUpperCase(char* str);
void toLowerCase(char* str);
double getArcAngle(const double& corda, const double& raio);
double getCatetoFromPitagoras(const double& hip, const double&  cat1);
DPoint calcularP3(
  const double& a, 
  const double& b, 
  const double& c, 
  const DPoint& p1, 
  const DPoint& p2
);
// Função para definir um bit específico em um byte
void setBit(uint8_t &byte, int position, bool value);
bool getBit(uint8_t byte, int position);
void setBit(uint16_t &byte, int position, bool value);
bool getBit(uint16_t byte, int position);
uint8_t getRandomic(uint8_t min = 0, uint8_t max = 255, uint8_t invalid = 255);
#endif // UTILS_H