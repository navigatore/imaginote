#include "segment.h"
#include <stdexcept>
#include "nvgmath.h"

Segment::Segment(const Coordinates2d &a, const Coordinates2d &b)
    : a(a),
      b(b),
      line(a, b),
      moreHorizontal(std::abs(a.x() - b.x()) > std::abs(a.y() - b.y())) {}

bool Segment::intersects(const Segment &other) const {
  if (!line.intersects(other.line)) {
    return false;
  }
  auto inter = line.getIntersectionPoint(other.line);
  return interPointInside(inter) && other.interPointInside(inter);
}

Coordinates2d Segment::getIntersectionPoint(const Segment &other) const {
  if (!intersects(other)) {
    throw std::runtime_error(
        "There is no intersection point between these two segments");
  }
  return line.getIntersectionPoint(other.line);
}

float Segment::distanceFromPoint(const Coordinates2d &point) const {
  auto perpendicularLineCrossingZero = line.getPerpendicularLineCrossingZero();
  auto perpendicularLineCrossingPoint =
      perpendicularLineCrossingZero.getParallelLineCrossingPoint(point);
  auto intersectionPoint =
      line.getIntersectionPoint(perpendicularLineCrossingPoint);
  if (interPointInside(intersectionPoint)) {
    return point.distance(intersectionPoint);
  }
  return std::min(point.distance(a), point.distance(b));
}

bool Segment::interPointInside(const Coordinates2d &point) const {
  if (moreHorizontal) {
    return (nvg::lessOrAlmostEqual(a.x(), point.x()) &&
            nvg::lessOrAlmostEqual(point.x(), b.x())) ||
           (nvg::lessOrAlmostEqual(b.x(), point.x()) &&
            nvg::lessOrAlmostEqual(point.x(), a.x()));
  }
  return (nvg::lessOrAlmostEqual(a.y(), point.y()) &&
          nvg::lessOrAlmostEqual(point.y(), b.y())) ||
         (nvg::lessOrAlmostEqual(b.y(), point.y()) &&
          nvg::lessOrAlmostEqual(point.y(), a.y()));
}
