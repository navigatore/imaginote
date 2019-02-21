#ifndef SEGMENT_H
#define SEGMENT_H

#include "coordinates2d.h"
#include "line.h"

class Segment {
 public:
  explicit Segment(const Coordinates2d &a, const Coordinates2d &b);

  bool intersects(const Segment &other) const;
  Coordinates2d getIntersectionPoint(const Segment &other) const;

 private:
  bool interPointInside(const Coordinates2d &point) const;

  Coordinates2d a, b;
  Line line;
  bool moreHorizontal;
};

#endif  // SEGMENT_H
