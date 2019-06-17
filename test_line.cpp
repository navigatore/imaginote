#include <gtest/gtest.h>
#include "line.h"

TEST(LineTest,
     givenVerticalLine_getParallelLineCrossingPoint_returnsVerticalLine) {
  Line verticalLine(Coordinates2d(0, 0), Coordinates2d(0, 1));
  ASSERT_TRUE(verticalLine.getParallelLineCrossingPoint(Coordinates2d(1, 1))
                  .isVertical());
}
