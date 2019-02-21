#include "angle.h"
#include "nvgmath.h"

Angle::Angle(float angle) : value(mod360(angle)) {}

Angle &Angle::operator+=(const Angle &other) { return operator+=(other.value); }

Angle &Angle::operator+=(float angle) {
  value = mod360(value + angle);
  return *this;
}

Angle &Angle::operator-=(float angle) { return operator+=(-angle); }

Angle Angle::operator+(const Angle &other) const {
  mod360(value + other.value);
  return Angle(value + other.value);
}

Angle Angle::operator-(const Angle &other) const { return *this + (-other); }

Angle Angle::operator/(float x) const { return Angle(this->value / x); }

Angle Angle::operator*(float x) const { return Angle(this->value * x); }

bool Angle::operator==(const Angle &other) const {
  return nvg::almostEqual(value, other.value);
}

bool Angle::inRange(const Angle &begin, const Angle &end) const {
  if (begin.value > end.value) {
    return !inRange(end, begin);
  }
  return value >= begin.value && value < end.value;
}

float Angle::getRad() const { return value * 3.1415f / 180.0f; }

int Angle::getQtAngle() const { return static_cast<int>(value * 16); }

std::string Angle::str() const { return std::to_string(value) + " deg"; }

Angle Angle::operator-() const { return Angle(-value); }
