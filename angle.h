#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <cmath>
#include <string>

class Angle {
 public:
  explicit Angle(float angle = 0.0F);

  static constexpr float fullAngle = 360.0F;

  Angle& operator+=(const Angle& other);
  Angle& operator+=(float angle);
  Angle& operator-=(float angle);

  Angle operator-() const;
  Angle operator+(const Angle& other) const;
  Angle operator-(const Angle& other) const;
  Angle operator/(float x) const;
  Angle operator*(float x) const;
  bool operator==(const Angle& other) const;

  [[nodiscard]] bool inRange(const Angle& begin, const Angle& end) const;

  [[nodiscard]] float getRad() const;
  [[nodiscard]] float getDeg() const;
  [[nodiscard]] int getQtAngle() const;
  [[nodiscard]] std::string str() const;

 private:
  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive& archive, const unsigned int /*version*/) {
    archive& value;
  }
  float value;
};
