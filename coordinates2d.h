#ifndef COORDINATES2D_H
#define COORDINATES2D_H

class Coordinates2d {
 public:
  Coordinates2d(float x = 0, float y = 0);

  float distance(const Coordinates2d &other) const;

  float x, y;
};

#endif  // COORDINATES2D_H
