#include "Utils.h"
#include <Arduino.h>
#include <math.h>  // Para usar funções trigonométricas como asin


double getArcAngle(const double& corda, const double& raio) {
    // Verifica se os parâmetros são válidos para evitar erros
    if (corda > 2 * raio) {
        // Retorna -1 para indicar erro, pois a corda não pode ser maior que o diâmetro
        return -1.0;
    }

    // Calcula o ângulo central em radianos
    double anguloRadianos = 2.0 * asin(corda / (2.0 * raio));

    // Converte o ângulo de radianos para graus
    double anguloGraus = anguloRadianos * 180.0 / M_PI;

    return anguloGraus;
};

// Função para calcular o ponto médio entre dois pontos
DPoint calcularPontoMedio(const DPoint& p1, const DPoint& p2) {
  DPoint pontoMedio;
  pontoMedio.x = (p1.x + p2.x) / 2.0;
  pontoMedio.y = (p1.y + p2.y) / 2.0;
  return pontoMedio;
}

// Função para calcular P3 com base nos comprimentos dos lados e nas coordenadas de P1 e P2
DPoint calcularP3(
  const double& a, 
  const double& b, 
  const double& c, 
  const DPoint& p1, 
  const DPoint& p2
) {
  DPoint p3;
  
  // Encontrar o ponto médio entre P1 e P2
  DPoint pontoMedio = calcularPontoMedio(p1, p2);

  // Resolver o sistema de equações para encontrar P3
  double d = sqrt(pow(p2.x - p1.x, 2.0) + pow(p2.y - p1.y, 2.0));  // Distância entre P1 e P2 (lado a)

  // Coordenadas de P3 (x3, y3)
  // Primeiro encontrar a distância entre P1 e P3, e P2 e P3
  double h = sqrt(pow(b, 2.0) - pow(d / 2.0, 2.0));  // Altura do triângulo em relação à base P1-P2

  // Calcular P3 usando simetria com o ponto médio e a altura
  // Direções ortogonais à reta P1P2
  double dx = (p2.y - p1.y) / d;
  double dy = (p2.x - p1.x) / d;

  // P3 estará em uma das duas posições possíveis: acima ou abaixo da linha P1-P2
  // Podemos escolher qualquer um dos dois (aqui assumimos uma direção acima da reta)
  p3.x = pontoMedio.x - h * dx;
  p3.y = pontoMedio.y + h * dy;

  return p3;
};

double getCatetoFromPitagoras(const double& hip, const double& cat1) {
  return sqrt(abs(pow(hip,2.0) - pow(cat1,2.0)));
}

String boolToString(const bool& v) {
  return v == true ? "true" : "false";
}

void toUpperCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Função para definir um bit específico em um byte
void setBit(uint8_t &byte, int position, bool value) {
    if (value) {
        byte |= (1 << position);  // Define o bit para 1
    } else {
        byte &= ~(1 << position); // Limpa o bit para 0
    }
}

// Função para obter o valor de um bit específico
bool getBit(uint8_t byte, int position) {
    return (byte >> position) & 1;
}

void setBit(uint16_t &byte, int position, bool value) {
    if (value) {
        byte |= (1 << position);  // Define o bit para 1
    } else {
        byte &= ~(1 << position); // Limpa o bit para 0
    }
}

// Função para obter o valor de um bit específico
bool getBit(uint16_t byte, int position) {
    return (byte >> position) & 1;
}

uint8_t getRandomic(uint8_t min, uint8_t max, uint8_t invalid) {
  uint8_t result = invalid;  
  while(result == invalid) {
    result = random(min,max);
  }
  return result;
}