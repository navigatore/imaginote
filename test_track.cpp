#include <gtest/gtest.h>
#include "track.h"

using namespace std::chrono_literals;

TEST(TrackTest, addingPositionIncreasesDuration) {
  Track track;
  auto initialDuration = track.getDuration();
  track.addPosition(Coordinates2d(1, 2));
  ASSERT_NE(initialDuration, track.getDuration());
}

TEST(TrackTest, afterResetDurationIsZero) {
  Track track;
  track.addPosition(Coordinates2d(1, 3));
  track.reset();
  ASSERT_EQ(track.getDuration(), 0ms);
}
