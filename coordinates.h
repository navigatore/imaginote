#pragma once
#include <cmath>
#include <limits>
#include <string>
#include "coordinates2d.h"

class Coordinates {
 public:
  explicit Coordinates(float x = 0, float y = 0, float z = 0);
  explicit Coordinates(const Coordinates2d& other);

  operator Coordinates2d() const;

  bool operator==(const Coordinates& other) const;
  bool operator!=(const Coordinates& other) const;
  Coordinates operator-() const;

  [[nodiscard]] float distance2d(const Coordinates& other) const;
  [[nodiscard]] static float distance2d(const Coordinates& first,
                                        const Coordinates& second);
  [[nodiscard]] std::string str() const;

  [[nodiscard]] const float& x() const;
  float& x();
  [[nodiscard]] const float& y() const;
  float& y();
  [[nodiscard]] const float& z() const;
  float& z();

 private:
  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive& archive, const unsigned int /*version*/) {
    archive& _x;
    archive& _y;
    archive& _z;
  }

  float _x, _y, _z;
};
