#include <gtest/gtest.h>
#include "space.h"

TEST(SpaceTest, afterCreatingEmptySpaceEveryFieldHasZeroHeight) {
  Space space;
  space.createEmptySpace(4, 4);
  for (const auto& row : space.getFields()) {
    for (const auto& field : row) {
      ASSERT_EQ(field.height(), 0);
    }
  }
}

TEST(SpaceTest, afterCreatingEmptySpaceWidthOfEveryRowIsCorrect) {
  Space space;
  space.createEmptySpace(2, 3);
  for (const auto& row : space.getFields()) {
    ASSERT_EQ(row.size(), 2);
  }
}

TEST(SpaceTest, afterCreatingEmptySpaceHeightIsCorrect) {
  Space space;
  space.createEmptySpace(2, 3);
  ASSERT_EQ(space.getFields().size(), 3);
}

TEST(SpaceTest,
     afterCreatingEmptySpaceCoordinatesAreCorrectOnFieldOtherThanTopLeft) {
  Space space;
  space.createEmptySpace(2, 3);
  ASSERT_EQ(space.getFields()[2][1].crds(), Coordinates(1, 0, 2));
}
