#include <gtest/gtest.h>
#include "nvgmath.h"

static constexpr float number = 23.4F;
static constexpr float closeBiggerNumber = 23.400001F;
static constexpr float biggerNumber = 23.5F;

TEST(NvgMathTest, givenIdenticalFloats_almostEqual_returnsTrue) {
  ASSERT_TRUE(nvg::almostEqual(number, number));
}

TEST(NvgMathTest, givenNotCloseFloats_almostEqual_returnsFalse) {
  ASSERT_FALSE(nvg::almostEqual(number, biggerNumber));
}

TEST(NvgMathTest, givenCloseFloats_almostEqual_returnsTrue) {
  ASSERT_NE(number, closeBiggerNumber);
  ASSERT_TRUE(nvg::almostEqual(number, closeBiggerNumber));
}

TEST(NvgMathTest, givenCloseFloatsFirstBigger_lessOrAlmostEqual_returnsTrue) {
  ASSERT_TRUE(nvg::lessOrAlmostEqual(number, closeBiggerNumber));
}

TEST(NvgMathTest, givenCloseFloatsSecondBigger_lessOrAlmostEqual_returnsTrue) {
  ASSERT_TRUE(nvg::lessOrAlmostEqual(closeBiggerNumber, number));
}

TEST(NvgMathTest,
     givenNotCloseFloatFirstBigger_lessOrAlmostEqual_returnsFalse) {
  ASSERT_FALSE(nvg::lessOrAlmostEqual(biggerNumber, number));
}

TEST(NvgMathTest,
     givenNotCloseFloatSecondBigger_lessOrAlmostEqual_returnsTrue) {
  ASSERT_TRUE(nvg::lessOrAlmostEqual(number, biggerNumber));
}
