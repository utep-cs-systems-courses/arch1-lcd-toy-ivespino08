#ifndef motion_included
#define motion_included

#include "shape.h"
#include <lcdutils.h>

typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

extern u_int bgColor;

void movLayerDraw(MovLayer *movLayers, Layer *layers);

void mlAdvance(MovLayer *ml, Region *fence, Region *slider1, Region *slider2);

void sliderAdvanceDown(MovLayer *sliders, Region *fence);

void sliderAdvanceUp(MovLayer *sliders, Region *fence);

#endif
