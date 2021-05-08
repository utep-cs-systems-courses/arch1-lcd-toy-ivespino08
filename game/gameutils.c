#include <shape.h>
#include <p2switches.h>
#include <lcddraw.h>
#include "motion.h"

void vec2AddSlider(Vec2 *result, const Vec2 *v1, const Vec2 *v2){
  result->axes[0] = v1->axes[0];
  result->axes[1] = v1->axes[1] + v2->axes[1];
}

void reset(Layer *ball, Vec2 *newPos){
  for(int axis = 0; axis < 2; axis++){ 
    ball->pos.axes[axis] = screenCenter.axes[axis];
    ball->posLast.axes[axis] = screenCenter.axes[axis];
    ball->posNext.axes[axis] = screenCenter.axes[axis];
    newPos->axes[axis] = screenCenter.axes[axis];

  }
  layerInit(ball);
  layerDraw(ball);
  drawString5x7(1,1, "player 1", COLOR_GREEN, COLOR_RED);
  drawString5x7(screenWidth-60,1, "player 2", COLOR_GREEN, COLOR_RED);
}
