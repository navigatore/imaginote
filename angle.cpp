#include "angle.h"
#include "nvgmath.h"

Angle::Angle(float angle) : value(nvg::positiveFmod(angle, fullAngle)) {}

Angle &Angle::operator+=(const Angle &other) { return operator+=(other.value); }

Angle &Angle::operator+=(float angle) {
  value = nvg::positiveFmod(value + angle, fullAngle);
  return *this;
}

Angle &Angle::operator-=(float angle) { return operator+=(-angle); }

Angle Angle::operator+(const Angle &other) const {
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

float Angle::getRad() const { return nvg::degToRad(value); }

float Angle::getDeg() const { return value; }

int Angle::getQtAngle() const { return nvg::degToQt(value); }

std::string Angle::str() const { return std::to_string(value) + " deg"; }

Angle Angle::operator-() const { return Angle(-value); }
