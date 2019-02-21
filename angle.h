#ifndef ANGLE_H
#define ANGLE_H

#include <cmath>
#include <string>

class Angle {
 public:
  explicit Angle(float angle = 0.0f);

  Angle& operator+=(const Angle& other);
  Angle& operator+=(float angle);
  Angle& operator-=(float angle);

  Angle operator-() const;
  Angle operator+(const Angle& other) const;
  Angle operator-(const Angle& other) const;
  Angle operator/(float x) const;
  Angle operator*(float x) const;
  bool operator==(const Angle& other) const;

  bool inRange(const Angle& begin, const Angle& end) const;

  float getRad() const;
  int getQtAngle() const;
  std::string str() const;

 private:
  static float mod360(float value) {
    return std::fmod(std::fmod(value, 360.0f) + 360.0f, 360.0f);
  }

 public:  // TODO: remove public access into value
  float value;
};

#endif  // ANGLE_H
