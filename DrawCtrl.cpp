#include "DrawCtrl.h"
#include "EvtCtrl.h"
#include "Gates.h"



static TextInfo DrawCtrl::drawCenteredText(
  const char* text, 
  const double& y = 0, 
  const double& centerX = TSCtrl::tft.width() * 1.0 / 2.0, 
  const uint8_t& textSize = 2, 
  const int& color = TFT_WHITE
) {
    // Defina o tamanho da fonte
    TSCtrl::tft.setTextSize(textSize);
    TSCtrl::tft.setTextColor(color);

    // Calcule a largura e altura do texto com base no tamanho da fonte
    double textWidth = strlen(text) * 6.0 * textSize;  // Cada caractere tem aproximadamente 6px de largura na fonte padrão
    double textHeight = 8.0 * textSize;  // A altura da fonte padrão é de aproximadamente 8px

    // Calcule as coordenadas para centralizar o texto
    double x = centerX - (textWidth / 2.0);
    
    // Desenhe o texto no TFT
    TSCtrl::tft.setCursor(x, y-3*textSize); //3 = half vertical size character at text size = 1
    TSCtrl::tft.print(text);

    // Retorne as informações do texto
    TextInfo result;
    result.x = x;
    result.y = y;
    result.w = textWidth;
    result.h = textHeight;
    return result;
};

static void DrawCtrl::drawSmoothArc(
  const double& x_center, 
  const double& y_center, 
  const double& radius, 
  const double& start_angle, 
  const double& end_angle, 
  const int& color
) {
  double angle;   // Ângulo atual em radianos
  double x_last = -1.0, y_last = -1.0;  // Últimas coordenadas desenhadas para garantir continuidade

  // Convertendo ângulos de graus para radianos
  double start_rad = start_angle * (PI / 180.0);
  double end_rad = end_angle * (PI / 180.0);

  // Caminhar ao longo do arco, aumentando o ângulo gradualmente
  for (angle = start_rad; angle <= end_rad; angle += 0.005) {
    double x = x_center + radius * cos(angle);  // Cálculo da coordenada X
    double y = y_center + radius * sin(angle);  // Cálculo da coordenada Y

    // Desenha o pixel apenas se for uma nova coordenada (evitar sobreposição)
    if (x != x_last || y != y_last) {
      TSCtrl::tft.drawPixel(x, y, color);  // Desenha o pixel na posição (x, y)
      x_last = x;
      y_last = y;
    }
  }
};

static CircleInfo DrawCtrl::drawArcFromArrow(
  const double& x1, 
  const double& y1, 
  const double& x2, 
  const double& y2, 
  const double& arcHeight, 
  const int& color
) {  
  DPoint p1,p2;
  p1.x = x1;
  p1.y = y1;
  p2.x = x2;
  p2.y = y2;

  double width = sqrt(pow(abs(x2-x1),2.0) + pow(abs(y2-y1),2.0));
  double r = 0.0;
  double baseAdjust = 0.0;
  double centralAngle = 270.0; //arco na horizontal, flexa na vertical y1 = y2 --- 
  if (y1 != y2) {
    if (x1 == x2) {// arco na vertical, flexa na horizontal |      
      centralAngle = 0.0;
      if (y1 > y2) {
        centralAngle = 180.0; //arco na vertical, flexa na horizontal, apontando para a esquerda
      }
    } else {
      double hT = abs(y1-y2); //cateto vertical
      double wT = abs(x1-x2); //cateto horizontal
      double a1 = atan(hT/wT) * (180.0 / M_PI); //angulo oposto ao cateto vertical
      //Serial.println("ht="+String(hT)+",wt="+String(wT)+",a1="+String(a1));
      if (x1 <= x2) { //flexa/arco para esquerda e para cima
        if (y1 <= y2) { //de 180 a 360
          centralAngle = 360.0 - (90-a1);
        } else {
          centralAngle = 180.0 + (90-a1);
        } 
      } else {//flexa do arco para esquerda
        if (y1 <= y2) {
          centralAngle = 90-a1;
        } else {
          centralAngle = 90.0 + (90-a1);
        }
      }
    }
  } else if (x1 > x2) {
    centralAngle = 90.0; //flexa apontando para baixo
  }
  if (arcHeight >= width/2.0) {
    r = width / 2.0;
    baseAdjust = arcHeight - r;
  } else {
    r = (arcHeight * arcHeight + pow(width/2.0,2.0)) / (2.0 * arcHeight);
  }
  double py2 = pow(r,2.0);
  py2 = pow(r,2.0) - pow(width /2.0,2.0);
  py2 = sqrt(py2);
  double arcAngle = getArcAngle(width,r);
  double startAngle = centralAngle-(arcAngle/2.0);

  DPoint pc = calcularP3(width,r,r,p1,p2);
  CircleInfo result;
  result.x = pc.x;
  result.y = pc.y;
  result.r = r;
  drawSmoothArc(pc.x,pc.y,r,startAngle,startAngle+arcAngle,color);    
  return result;
};

static void DrawCtrl::drawRoundedPlay(
  const double& cx,
  const double& cy,
  const double& r,
  const double& round,
  const int& backgroundColor,
  const int& color,
  const double& multiplier
) {
  double angulo1 = 0;  // Vértice apontando para a direita (ponta do play)
  double angulo2 = 2 * M_PI / 3;  // 120 graus (em radianos)
  double angulo3 = 4 * M_PI / 3;  // 240 graus (em radianos)

  //define os pontos do triangulo do play
  // Coordenadas dos três vértices (p1, p2, p3)
  double x1 = cx + r * cos(angulo1) * multiplier;
  double y1 = cy + r * sin(angulo1) * multiplier;
  
  double x2 = cx + r * cos(angulo2) * multiplier;
  double y2 = cy + r * sin(angulo2) * multiplier;
  
  double x3 = cx + r * cos(angulo3) * multiplier;
  double y3 = cy + r * sin(angulo3) * multiplier;
  

  //desenha o play
  TSCtrl::tft.fillTriangle(x1,y1, x2,y2, x3,y3 , color);
  if (round > 0) {
    //desenha os cantos arredondados
    TSCtrl::tft.fillCircle(x1,y1,round*1.5, backgroundColor);
    TSCtrl::tft.fillCircle(x1-round*1.8*multiplier,y1,round, color);
    TSCtrl::tft.fillCircle(x2,y2,round*1.5, backgroundColor);
    TSCtrl::tft.fillCircle(x2+round*multiplier,y2-round*1.75*multiplier,round, color);
    TSCtrl::tft.fillCircle(x3,y3,round*1.5, backgroundColor);
    TSCtrl::tft.fillCircle(x3+round*multiplier,y3+round*1.75*multiplier,round, color);
  }
}

static void DrawCtrl::drawComponent(
  uint8_t type,
  int16_t x,
  int16_t y,
  int16_t m1,
  int16_t m2,
  double& x2,
  double& y2,
  double& r1,
  double r2,
  uint16_t color,
  bool filled,
  char* text,
  uint16_t textColor
) {

  
  //draw
  if (type == 2) {//circle
    if (filled) {//1-filled
      TSCtrl::tft.fillCircle(x, y, r1, color);
    } else {
      TSCtrl::tft.drawCircle(x, y, r1, color);
    }
  } else if (type == 3) {//triangle    
    if (x == m1) {
      y2 = min(m2,y) + abs(m2-y)/2.0;
      x2 = x + sqrt(3.0) * (m2*1.0-y*1.0) / 2.0;
      r1 = abs(m2-y) / 2; //to use in circular click event to convenience
      //Serial.println("seted r1 "+String(r1));
    } else if (y == m2) {
      x2 = min(m1,x) + abs(m1-x)/2.0;
      y2 = y + sqrt(3.0) * (m1*1.0-x*1.0) / 2.0;
      r1 = abs(m1-x) / 2;
    } else {
      DPoint p1;
      DPoint p2;
      p1.x = x;
      p1.y = y;
      p2.x = m1;
      p2.y = m2;  
      double d = sqrt(pow(m1 - x, 2.0) + pow(m2 - y, 2.0));  // Distância entre P1 e P2 (lado a)    
      DPoint p3 = calcularP3(d,d,d,p1,p2);
      x2 = p3.x;
      y2 = p3.y;
      r1 = d / 2;
    }    
    if (filled) {//1-filled
      TSCtrl::tft.fillTriangle(x, y, m1,m2,x2,y2, color);
      if (r2 != 0) {
        //desenha os cantos arredondados
        double multiplier = x < x2 ? 1 : -1;
        TSCtrl::tft.fillCircle(x,y,r2*1.5, DEFAULT_BACKGROUND_COLOR);        
        TSCtrl::tft.fillCircle(m1,m2,r2*1.5, DEFAULT_BACKGROUND_COLOR);        
        
        
        if (multiplier > 0) {
          TSCtrl::tft.fillCircle(x2-r2*0.5,y2,r2*1.5, DEFAULT_BACKGROUND_COLOR);
          TSCtrl::tft.fillCircle(x+r2,y+r2*1.75,r2, color);
          TSCtrl::tft.fillCircle(m1+r2,m2-r2*1.75,r2, color);
          TSCtrl::tft.fillCircle(x2-r2*2,y2,r2, color);
        } else {
          TSCtrl::tft.fillCircle(x2-r2*0.5*multiplier,y2,r2*1.5, DEFAULT_BACKGROUND_COLOR);
          TSCtrl::tft.fillCircle(x+r2*multiplier,y+r2*1.75*multiplier,r2, color);
          TSCtrl::tft.fillCircle(m1+r2*multiplier,m2-r2*1.75*multiplier,r2, color);
          TSCtrl::tft.fillCircle(x2-r2*2*multiplier,y2,r2, color);
        }
      }
    } else {
      TSCtrl::tft.drawTriangle(x, y, m1,m2,x2,y2, color);
    }
  } else { //rectangle
    if (filled) {
      if (r2 != 0) {
        TSCtrl::tft.fillRoundRect(x, y, m1, m2, r2, color);
      } else {
        TSCtrl::tft.fillRect(x, y, m1, m2, color);
      }
    } else {
      if (r2 != 0) {
        TSCtrl::tft.drawRoundRect(x, y, m1, m2, r2, color);
      } else {
        TSCtrl::tft.drawRect(x, y, m1, m2, color);
      }
    }
  }
  if (text != nullptr) {
    drawCenteredText(text,y+m2/2,x+m1/2,DEFAULT_TEXT_SIZE,textColor);
  }
}

static ClickEvent* DrawCtrl::drawClickable(
  void (*staticOnClick)(),
  ICallback* onClickCallback,
  uint8_t type,
  int16_t x,
  int16_t y,
  int16_t m1,
  int16_t m2,
  double r1,
  double r2,
  uint16_t color,
  bool filled,
  char* text,
  uint16_t textColor
) {

  double x2 = 0;
  double y2 = 0;
  
  drawComponent(
    type,
    x,
    y,
    m1,
    m2,
    x2,
    y2,
    r1,
    r2,
    color,
    filled,
    text,
    textColor
  );

   //create event
  ClickEvent* result = nullptr;
  if(staticOnClick != nullptr) {
    result = new ClickEvent();    
    result->staticOnClick = staticOnClick;
  } else if (onClickCallback != nullptr) {
    result = new ClickEvent();
    result->onClickCallback = onClickCallback;
  }
  if (result != nullptr) {
    result->type = type;
    result->x = x;
    result->y = y;  
    if (type == 2) {//circle
      result->m1 = r1;
    } else if (type == 3) { //triangle
      result->x = x+((x2-x)/2);
      result->y = y+((m2-y)/2);
      result->m1 = r1;
    } else {
      result->m1 = m1;
      result->m2 = m2;
    }
  }

  return result;
}


static void DrawCtrl::drawOption(
  const double& pX,
  const double& pY, 
  const double& pR, 
  const int& pColor, 
  const bool& hasPlay, 
  const char* pText, 
  const void (*pStaticOnClick)(), 
  const ICallback* pOnClick
){
  ClickEvent* ev = drawClickable(
    pStaticOnClick,
    pOnClick,
    2,
    pX,
    pY,    
    0,
    0,
    pR,
    0,
    pColor,
    true
  );
  EvtCtrl::addScreenEvent(ev);

  //draw button border
  TSCtrl::tft.drawCircle(pX, pY, pR+5, pColor);
  TSCtrl::tft.drawCircle(pX, pY, pR+6, pColor);

  if (hasPlay) {
    drawRoundedPlay(pX, pY, pR*0.5,pR*0.05,pColor,DEFAULT_BACKGROUND_COLOR);
  }

  if (pText != nullptr) {
    drawCenteredText(pText,pY+pR+20,pX+2);
  }
}

static void DrawCtrl::drawGateNegation(const Gate* g) {
  if (getBit(g->packedFlags,8)) {//hasNot    
    TSCtrl::tft.drawCircle(g->x+g->w/2.0,g->y-g->h-g->notRadius,g->notRadius,TFT_WHITE);
  }
}

static void DrawCtrl::drawGateOutputConnector(const Gate* g) {
  TSCtrl::tft.drawLine(
    g->outputX,
    g->outputY,
    g->outputX,
    g->y-g->h-g->notRadius*2,
    TFT_WHITE
  );
}

static void DrawCtrl::drawGateInputsButtons(Gate* g){
  if (getBit(g->packedFlags,5)) {//hasInputButtons
    for (uint8_t i = 0; i < g->inputCount; i++) {
      auto f = [g,i](){
        invertGateInput(g,i);
      };
      EvtCtrl::addTransitoryEvent(
        drawClickable(
          nullptr,
          new LambdaCallback<decltype(f)>(f),
          2,
          g->firstInputX+i*g->inputSpaceBetwenn,
          g->firstInputY,
          0,
          0,
          g->inputRadius,
          0,
          !getBit(g->packedInputs,i) ? DEFAULT_GATE_INPUT_OFF_COLOR : DEFAULT_GATE_INPUT_ON_COLOR
        )
      );
    }
  }
}

static void DrawCtrl::drawGateOutputButton(Gate* g){
  if (getBit(g->packedFlags,7)) {//visibleOtput  
    TSCtrl::tft.fillCircle(
      g->outputX, 
      g->outputY, 
      g->inputRadius,
      getBit(g->packedInputs,7) ? DEFAULT_GATE_INPUT_ON_COLOR : DEFAULT_GATE_INPUT_OFF_COLOR
    );
  }
}

static void DrawCtrl::drawAnd(Gate* g) {
 
  //body
  double arcHeight = g->h/2.0;
  TSCtrl::tft.drawLine(g->x,g->y,g->x+g->w,g->y,TFT_WHITE); //base line
  TSCtrl::tft.drawLine(g->x,g->y-g->h+arcHeight,g->x,g->y,TFT_WHITE); //left line
  TSCtrl::tft.drawLine(g->x+g->w,g->y-g->h+arcHeight,g->x+g->w,g->y,TFT_WHITE); //rigth line        
  drawArcFromArrow(g->x,g->y-g->h+arcHeight,g->x+g->w,g->y-g->h+arcHeight,arcHeight,TFT_WHITE); //top arc   

  //input connectors
  for (uint8_t i = 0; i < g->inputCount; i++) {
    TSCtrl::tft.drawLine(
      g->firstInputX+i*g->inputSpaceBetwenn,
      g->y,
      g->firstInputX+i*g->inputSpaceBetwenn,
      g->firstInputY,
      TFT_WHITE
    );
  }

  drawGateNegation(g);
  
  drawGateOutputConnector(g);

  drawGateInputsButtons(g);

  calcOutputState(g);

  drawGateOutputButton(g);
}


static void DrawCtrl::drawOr(Gate* g) {

  //body
  double arcHeight = g->h * DEFAULT_GATE_BASE_OR_ARC_HEIGHT_PERC;

  double baseArcHeight = g->h * DEFAULT_GATE_BASE_ARC_HEIGHT_ASPECT_RATIO;
  CircleInfo baseArc;

  //draw curved base  
  baseArc = drawArcFromArrow(g->x,g->y,g->x+g->w,g->y,baseArcHeight,TFT_WHITE);  

  //exclusive
  if (getBit(g->packedFlags,9)) {//9-exclusive
    drawArcFromArrow(g->x,g->y+g->h * DEFAULT_GATE_EXCLUSIVE_SPACE_PERC,g->x+g->w,g->y+g->h * DEFAULT_GATE_EXCLUSIVE_SPACE_PERC,baseArcHeight,TFT_WHITE);
  }

  //laterals
  TSCtrl::tft.drawLine(g->x,g->y-g->h+arcHeight,g->x,g->y,TFT_WHITE); //left line
  TSCtrl::tft.drawLine(g->x+g->w,g->y-g->h+arcHeight,g->x+g->w,g->y,TFT_WHITE); //rigth line 

  //connectors  
  double newConnectorSize,newPos;
  int16_t pcX = 0;
  for (uint8_t i = 0; i < g->inputCount * 1.0 / 2; i++) {
    pcX = g->firstInputX+i*g->inputSpaceBetwenn;
    newConnectorSize = getCatetoFromPitagoras(baseArc.r,baseArc.x-pcX); 
    newPos = baseArc.y-newConnectorSize;
    newConnectorSize = newConnectorSize - (baseArc.y - g->firstInputY);
    TSCtrl::tft.drawLine(
      pcX,
      newPos,
      pcX,
      newPos+newConnectorSize,
      TFT_WHITE
    );
    if ((i+1)>g->inputCount*1.0/2) break;
    pcX = g->firstInputX+(g->inputCount-1-i)*g->inputSpaceBetwenn;
    TSCtrl::tft.drawLine(
      pcX,
      newPos,
      pcX,
      newPos+newConnectorSize,
      TFT_WHITE
    );
  }

  //top arcs
  double arcHeight2 = sqrt(pow((g->x+(g->w/2.0)) - g->x, 2.0) + pow((g->y-g->h+arcHeight) - (g->y-g->h), 2.0));  // Distância entre P1 e P2 (lado a)
  arcHeight2 = arcHeight2 / 15; //divide o circulo em 15 partes, 
  drawArcFromArrow(g->x,g->y-g->h+arcHeight,g->x+(g->w/2.0),g->y-g->h,arcHeight2,TFT_WHITE);
  drawArcFromArrow(g->x+(g->w/2.0),g->y-g->h,g->x+g->w,g->y-g->h+arcHeight,arcHeight2,TFT_WHITE);

  //negation
  drawGateNegation(g);

  drawGateOutputConnector(g);

  drawGateInputsButtons(g);

  calcOutputState(g);

  drawGateOutputButton(g);
}


static void DrawCtrl::drawNot(Gate* g) {
  //body
  TSCtrl::tft.drawTriangle(g->x,g->y,g->x+g->w,g->y,g->x+g->w/2,g->y-g->h, TFT_WHITE);

  //input connector
  TSCtrl::tft.drawLine(
    g->firstInputX,
    g->y,
    g->firstInputX,
    g->firstInputY,
    TFT_WHITE
  );

  drawGateNegation(g);

  drawGateOutputConnector(g);

  drawGateInputsButtons(g);

  calcOutputState(g);

  drawGateOutputButton(g);
}



static void DrawCtrl::drawGate(Gate* g) {
  if (g != nullptr) {
    switch(g->type) {
      case 1://or
      case 4://nor
      case 5://xor
      case 6://xnor
        drawOr(g);   
        break;
      case 2://not
        drawNot(g);   
        break;
      default://0-and,nand
        drawAnd(g);   
    }
  }
}
