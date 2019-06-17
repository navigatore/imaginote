#include <gtest/gtest.h>
#include <cmath>
#include "path.h"

class PathOfTwoSegmentsTest : public ::testing::Test {
 protected:
  void SetUp() override {
    path.addNode(Coordinates2d(0, 0));
    path.addNode(Coordinates2d(0, 3));
    path.addNode(Coordinates2d(3, 3));
  }

  Path path;
};

TEST_F(PathOfTwoSegmentsTest,
       givenPointCloserToFirstSegment_distance_isCorrect) {
  Coordinates2d point(1, 1);
  ASSERT_FLOAT_EQ(path.distance(point), 1);
}

TEST_F(PathOfTwoSegmentsTest,
       givenPointEquallyDistantFromBothSegments_distance_isCorrect) {
  Coordinates2d point(1, 2);
  ASSERT_FLOAT_EQ(path.distance(point), 1);
}

TEST_F(PathOfTwoSegmentsTest,
       givenPointOnSegmentsIntersection_distance_isZero) {
  Coordinates2d point(0, 3);
  ASSERT_FLOAT_EQ(path.distance(point), 0);
}

TEST_F(PathOfTwoSegmentsTest,
       givenPointCloseToSecondSegmentEnd_distance_isCorrect) {
  Coordinates2d point(4, 2);
  ASSERT_FLOAT_EQ(path.distance(point), std::sqrt(2.0F));
}
