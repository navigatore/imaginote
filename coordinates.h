#ifndef COORDINATES_H
#define COORDINATES_H

#include <cmath>
#include <limits>
#include <string>
#include "coordinates2d.h"

class Coordinates {
 public:
  Coordinates(float x = 0, float y = 0, float z = 0);

  operator Coordinates2d() const;

  bool operator==(const Coordinates& other);
  Coordinates operator-() { return Coordinates(-_x, -_y, -_z); }

  [[nodiscard]] float distance2d(const Coordinates& other) const;
  [[nodiscard]] std::string str() const;

  [[nodiscard]] const float& x() const;
  float& x();
  [[nodiscard]] const float& y() const;
  float& y();
  [[nodiscard]] const float& z() const;
  float& z();

 private:
  float _x, _y, _z;
};

#endif  // COORDINATES_H
