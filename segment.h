#pragma once
#include "coordinates2d.h"
#include "line.h"

class Segment {
 public:
  explicit Segment(const Coordinates2d &a, const Coordinates2d &b);

  [[nodiscard]] bool intersects(const Segment &other) const;
  [[nodiscard]] Coordinates2d getIntersectionPoint(const Segment &other) const;
  [[nodiscard]] float distanceFromPoint(const Coordinates2d &point) const;

 private:
  [[nodiscard]] bool interPointInside(const Coordinates2d &point) const;

  Coordinates2d a, b;
  Line line;
  bool moreHorizontal;
};
