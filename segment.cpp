#include "segment.h"
#include <stdexcept>

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

bool Segment::interPointInside(const Coordinates2d &point) const {
  if (moreHorizontal) {
    return (a.x() <= point.x() && point.x() <= b.x()) ||
           (b.x() <= point.x() && point.x() <= a.x());
  }
  return (a.y() <= point.y() && point.y() <= b.y()) ||
         (b.y() <= point.y() && point.y() <= a.y());
}
