#include "coordinates2d.h"
#include <cmath>

Coordinates2d::Coordinates2d(float x, float y) : _x(x), _y(y) {}

float Coordinates2d::distance(const Coordinates2d &other) const {
  return std::sqrt((_x - other._x) * (_x - other._x) +
                   (_y - other._y) * (_y - other._y));
}

float &Coordinates2d::x() { return _x; }

float &Coordinates2d::y() { return _y; }

const float &Coordinates2d::y() const { return _y; }

const float &Coordinates2d::x() const { return _x; }
