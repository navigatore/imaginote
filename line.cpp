#include "line.h"
#include <stdexcept>
#include "nvgmath.h"

Line::Line(const Coordinates2d &first, const Coordinates2d &second) {
  if (nvg::almostEqual(first.x, second.x)) {
    a = b = 0;
    c = first.x;
    vertical = true;
  } else {
    a = (first.y - second.y) / (first.x - second.x);
    b = first.y - a * first.x;
    c = 0;
    vertical = false;
  }
}

bool Line::intersects(const Line &other) const {
  if (vertical && other.vertical) {
    return false;
  }
  if (vertical || other.vertical) {
    return true;
  }
  return !nvg::almostEqual(a, other.a);
}

Coordinates2d Line::getIntersectionPoint(const Line &other) const {
  if (!intersects(other)) {
    throw std::runtime_error(
        "There is not intersection point between perpendicular lines");
  }

  Coordinates2d inter;
  if (vertical) {
    inter.x = c;
    inter.y = other.a * inter.x + other.b;
  } else if (other.vertical) {
    inter.x = other.c;
    inter.y = a * other.c + b;
  } else {
    inter.x = (other.b - b) / (a - other.a);
    inter.y = a * inter.x + b;
  }
  return inter;
}
