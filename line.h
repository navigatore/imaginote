#ifndef LINE_H
#define LINE_H

#include "coordinates2d.h"

class Line {
 public:
  Line(const Coordinates2d &first, const Coordinates2d &second);

  [[nodiscard]] bool intersects(const Line &other) const;
  [[nodiscard]] Coordinates2d getIntersectionPoint(const Line &other) const;

 private:
  float a, b, c;
  bool vertical;
};

#endif  // LINE_H
