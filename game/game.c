#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include "motion.h"

#define GREEN_LED BIT6

AbRect rect0 =  {abRectGetBounds, abRectCheck, {1,12}};

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2 - 10, screenHeight/2 - 10}
};

Layer fieldLayer = {
  (AbShape*)&fieldOutline,
  {screenWidth/2, screenHeight/2},
  {0,0},{0,0},
  COLOR_BLACK,
  0
};

Layer layer2 = {
  (AbShape *)&rect0,
  {2, screenHeight/2},
  {0,0}, {0,0},
  COLOR_GREEN,
  &fieldLayer
};

Layer layer1 = {
  (AbShape*)&rect0,
  {screenWidth-3, screenHeight/2},
  {0,0}, {0,0},
  COLOR_WHITE,
  &layer2
};

Layer layer0 = {
  (AbShape*)&circle3,
  {screenWidth/2, screenHeight/2},
  {0,0},{0,0},
  COLOR_WHITE,
  &layer1
};

MovLayer ml0 = {&layer0, {1,2}, 0};

int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */

u_int bgColor = COLOR_RED;

int main(){
  configureClocks();
  lcd_init();
  shapeInit();
  p2sw_init(1);

  layerInit(&layer0);
  layerDraw(&layer0);
  
  layerGetBounds(&fieldLayer, &fieldFence);
  
  enableWDTInterrupts();
  or_sr(0x8);

  for(;;){
    while(!redrawScreen){
      P1OUT &= ~GREEN_LED;
      or_sr(0x10);
    }
    P1OUT |= GREEN_LED;
    redrawScreen = 0;
    movLayerDraw(&ml0, &layer0);
  }
}

void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;
  count++;
  if(count == 15){
    mlAdvance(&ml0, &fieldFence);
    if(p2sw_read()){
      redrawScreen = 1;
    }
    count = 0;
  }
  P1OUT &= ~GREEN_LED;
}
