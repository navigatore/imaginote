#include <gtest/gtest.h>
#include "duration.h"

using namespace std::chrono_literals;

TEST(DurationTest, givenTimeUnderSecond_toString_producesOneProperElement) {
  auto duration = Duration(345ms);
  ASSERT_EQ(duration.toString(), "345 ms");
}

TEST(DurationTest, givenTimeAboveSecond_toString_producesTwoProperElements) {
  auto duration = Duration(2345ms);
  ASSERT_EQ(duration.toString(), "2 s  345 ms");
}
