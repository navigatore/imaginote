#ifndef NVGMATH_H
#define NVGMATH_H

#include <cmath>

namespace nvg {
constexpr float pi = static_cast<float>(M_PI);
constexpr float straightAngle = 180.0F;
constexpr float fullAngleDeg = 2.0F * pi;
constexpr unsigned int qtUnitsInDegree = 16;

constexpr bool almostEqual(float a, float b) {
  auto eps = std::numeric_limits<float>::epsilon();
  return std::abs(a - b) < eps;
}
constexpr float positiveFmod(float a, float b) {
  return std::fmod(std::fmod(a, b) + b, b);
}
constexpr float degToRad(float deg) { return deg * pi / straightAngle; }
constexpr int degToQt(float deg) {
  return static_cast<int>(deg * qtUnitsInDegree);
}

}  // namespace nvg
#endif  // NVGMATH_H
