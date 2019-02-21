#include "nvgmath.h"
#include <cmath>
#include <limits>

bool nvg::almostEqual(float a, float b) {
  auto eps = std::numeric_limits<float>::epsilon();
  return std::abs(a - b) < eps;
}
