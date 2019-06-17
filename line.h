#ifndef LINE_H
#define LINE_H

#include "coordinates2d.h"

class Line {
 public:
  Line(const Coordinates2d &first, const Coordinates2d &second);
  Line(float a, float b);

  [[nodiscard]] bool intersects(const Line &other) const;
  [[nodiscard]] bool isVertical() const;
  [[nodiscard]] Coordinates2d getIntersectionPoint(const Line &other) const;
  [[nodiscard]] Line getPerpendicularLineCrossingZero() const;
  [[nodiscard]] Line getParallelLineCrossingPoint(
      const Coordinates2d &point) const;

 private:
  float a, b, c;
  bool vertical{};
};

#endif  // LINE_H
