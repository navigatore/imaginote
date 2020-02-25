#include "line.h"

#include <stdexcept>

#include "nvgmath.h"

Line::Line(const Coordinates2d &first, const Coordinates2d &second) {
  if (nvg::almostEqual(first.x(), second.x())) {
    a = b = 0;
    c = first.x();
    vertical = true;
  } else {
    a = (first.y() - second.y()) / (first.x() - second.x());
    b = first.y() - a * first.x();
    c = 0;
    vertical = false;
  }
}

Line::Line(float a, float b) : a(a), b(b), c(0) {}

bool Line::intersects(const Line &other) const {
  if (vertical && other.vertical) {
    return false;
  }
  if (vertical || other.vertical) {
    return true;
  }
  return !nvg::almostEqual(a, other.a);
}

bool Line::isVertical() const { return vertical; }

Coordinates2d Line::getIntersectionPoint(const Line &other) const {
  if (!intersects(other)) {
    throw std::runtime_error(
        "There is no intersection point between parallel lines");
  }

  Coordinates2d inter;
  if (vertical) {
    inter.x() = c;
    inter.y() = other.a * inter.x() + other.b;
  } else if (other.vertical) {
    inter.x() = other.c;
    inter.y() = a * other.c + b;
  } else {
    inter.x() = (other.b - b) / (a - other.a);
    inter.y() = a * inter.x() + b;
  }
  return inter;
}

Line Line::getPerpendicularLineCrossingZero() const {
  if (vertical) {
    return Line(Coordinates2d(0, 0), Coordinates2d(1, 0));
  }
  if (nvg::almostEqual(a, 0)) {
    return Line(Coordinates2d(0, 0), Coordinates2d(0, 1));
  }
  auto perpendicularA = -1.0F / a;
  auto perpendicularB = 0.0F;
  return Line(perpendicularA, perpendicularB);
}

Line Line::getParallelLineCrossingPoint(const Coordinates2d &point) const {
  Line parallel(0, 0);
  if (vertical) {
    parallel.vertical = vertical;
    parallel.c = point.x();
    return parallel;
  }
  parallel.a = a;
  parallel.b = point.y() - a * point.x();
  return parallel;
}
