#include "coordinates.h"

Coordinates::Coordinates(float x, float y, float z) : _x(x), _y(y), _z(z) {}

Coordinates::Coordinates(const Coordinates2d &other)
    : _x(other.x()), _y(0), _z(other.y()) {}

std::string Coordinates::str() const {
  return "X: " + std::to_string(_x) + "  Y: " + std::to_string(_y) +
         "  Z: " + std::to_string(_z);
}

const float &Coordinates::x() const { return _x; }

float &Coordinates::x() { return _x; }

const float &Coordinates::y() const { return _y; }

float &Coordinates::y() { return _y; }

const float &Coordinates::z() const { return _z; }

float &Coordinates::z() { return _z; }

bool Coordinates::operator==(const Coordinates &other) const {
  auto eps = std::numeric_limits<float>::epsilon();
  return std::abs(_x - other._x) < eps && std::abs(_y - other._y) < eps &&
         std::abs(_z - other._z) < eps;
}

float Coordinates::distance2d(const Coordinates &other) const {
  return distance2d(*this, other);
}

float Coordinates::distance2d(const Coordinates &first,
                              const Coordinates &second) {
  return std::sqrt((first._x - second._x) * (first._x - second._x) +
                   (first._z - second._z) * (first._z - second._z));
}

Coordinates::operator Coordinates2d() const { return Coordinates2d(_x, _z); }
