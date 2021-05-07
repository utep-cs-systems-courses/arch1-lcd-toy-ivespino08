#include <shape.h>
#include <p2switches.h>

void vec2AddSlider(Vec2 *result, const Vec2 *v1, const Vec2 *v2){
  result->axes[0] = v1->axes[0];
  result->axes[1] = v1->axes[1] + v2->axes[1];
}
