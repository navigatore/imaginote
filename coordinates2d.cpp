#include "coordinates2d.h"
#include <cmath>

Coordinates2d::Coordinates2d(float x, float y) : x(x), y(y) {}

float Coordinates2d::distance(const Coordinates2d &other) const {
  return std::sqrt((x - other.x) * (x - other.x) +
                   (y - other.y) * (y - other.y));
}
