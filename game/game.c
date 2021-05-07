#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

#define GREEN_LED BIT6

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

int redrawScreen = 1;

int main(){
  configureClocks();
  lcd_init();
  shapeInit();
  p2sw_init(1);

  enableWDTInterrupts();
  or_sr(0x8);

  clearScreen(COLOR_RED);
  
  for(;;){
    while(!redrawScreen){
      P1OUT &= ~GREEN_LED;
      or_sr(0x10);
    }
    P1OUT |= GREEN_LED;
    redrawScreen = 0;
    layerDraw(&layer2);
  }
}

void wdt_c_handler()
{
  static char state = 0;
  static short count = 0;
  P1OUT |= GREEN_LED;
  count++;
  if(count == 1000){
    switch(state){
    case 0:
      bgColor = COLOR_BLUE;
      state = 1;
      break;
    case 1:
      bgColor = COLOR_VIOLET;
      state = 2;
      break;
    case 2:
      bgColor = COLOR_RED;
      state = 0;
      break;
    }
    redrawScreen = 1;
    count = 0;
  }
  P1OUT &= ~GREEN_LED;
}
