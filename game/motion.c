#include <libTimer.h>
#include <shape.h>
#include "motion.h"
#include <lcdutils.h>
#include "gameutils.h"
#include "buzzer.h"

void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}

void mlAdvance(MovLayer *ml, Region *fence, Region *slider1, Region *slider2)
{
  Vec2 newPos;
  Region shapeBoundary;
  
  vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
  abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
  if ((shapeBoundary.topLeft.axes[1] < fence->topLeft.axes[1]) ||
      (shapeBoundary.botRight.axes[1] > fence->botRight.axes[1])) {
    buzzer_set_period(1000);
    int velocity = ml->velocity.axes[1] = -ml->velocity.axes[1];
    newPos.axes[1] += (2*velocity);
  }	/**< if outside of fence */

  if ((shapeBoundary.topLeft.axes[0] < fence->topLeft.axes[0]) ||
	(shapeBoundary.botRight.axes[0] > fence->botRight.axes[0])) {
    reset(ml->layer, &newPos);
  }
  
  if(((shapeBoundary.botRight.axes[0] > slider1->topLeft.axes[0]) &&
      (shapeBoundary.botRight.axes[1] < slider1->botRight.axes[1]) &&
      (shapeBoundary.topLeft.axes[1] > slider1->topLeft.axes[1])) ||
     ((shapeBoundary.topLeft.axes[0] < slider2->botRight.axes[0]) &&
      (shapeBoundary.botRight.axes[1] < slider2->botRight.axes[1]) &&
      (shapeBoundary.topLeft.axes[1] > slider2->topLeft.axes[1]))){
    buzzer_set_period(1500);
    int velocity = ml->velocity.axes[0] = -ml->velocity.axes[0];
    newPos.axes[0] += (2*velocity);
  }
    
  ml->layer->posNext = newPos;
} /**< for ml */

void sliderAdvanceDown(MovLayer *sliders, Region *fence){
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; sliders; sliders = sliders->next) {
    vec2AddSlider(&newPos, &sliders->layer->posNext, &sliders->velocity);
    abShapeGetBounds(sliders->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	newPos.axes[axis] = sliders->layer->pos.axes[axis];
      }	/**< if outside of fence */
    } /**< for axis */
    sliders->layer->posNext = newPos;
  }
}

void sliderAdvanceUp(MovLayer *sliders, Region *fence){
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  Vec2 velocity = {-sliders->velocity.axes[0] , -sliders->velocity.axes[1]};
  for (; sliders; sliders = sliders->next) {
    vec2AddSlider(&newPos, &sliders->layer->posNext, &velocity);
    abShapeGetBounds(sliders->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	newPos.axes[axis] = sliders->layer->pos.axes[axis];
      }	/**< if outside of fence */
    } /**< for axis */
    sliders->layer->posNext = newPos;
  }
}
