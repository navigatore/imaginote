#pragma once
#include <cmath>

namespace nvg {
constexpr float pi = static_cast<float>(M_PI);
constexpr float straightAngle = 180.0F;
constexpr float fullAngleDeg = 2.0F * pi;
constexpr unsigned int qtUnitsInDegree = 16;
constexpr float epsilon = std::numeric_limits<float>::epsilon() * 100;

constexpr bool almostEqual(float a, float b) {
  return std::abs(a - b) < epsilon;
}

constexpr bool lessOrAlmostEqual(float a, float b) {
  return a < b || almostEqual(a, b);
}

constexpr float positiveFmod(float a, float b) {
  return std::fmod(std::fmod(a, b) + b, b);
}
constexpr float degToRad(float deg) { return deg * pi / straightAngle; }
constexpr int degToQt(float deg) {
  return static_cast<int>(deg * qtUnitsInDegree);
}

}  // namespace nvg
