#ifndef gameutils_included
#define gameutils_included

#include "shape.h"
#include "stateMachine.h"

void vec2AddSlider(Vec2 *result, const Vec2 *v1, const Vec2 *v2);

void reset(Layer *ball, Vec2 *newPos);

#endif
