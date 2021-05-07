#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <shape.h>
#include <abCircle.h>

AbRect rect0 =  {abRectGetBounds, abRectCheck, {1,12}};

u_int bgColor = COLOR_RED;

Layer layer0 = {
  (AbShape *)&rect0,
  {2, screenHeight/2},
  {0,0}, {0,0},
  COLOR_GREEN,
  0
};

Layer layer1 = {
  (AbShape*)&rect0,
  {screenWidth-3, screenHeight/2},
  {0,0}, {0,0},
  COLOR_WHITE,
  &layer0
};

Layer layer2 = {
  (AbShape*)&circle3,
  {screenWidth/2, screenHeight/2},
  {0,0},{0,0},
  COLOR_WHITE,
  &layer1
};

int main(){
  configureClocks();
  lcd_init();
  shapeInit();

  clearScreen(COLOR_RED);
  layerDraw(&layer2);

}
