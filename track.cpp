#include "track.h"

Track::Track(Duration timeBetweenPoints)
    : timeBetweenPoints(timeBetweenPoints) {}

void Track::addPosition(const Coordinates2d& point) {
  recPositions.push_back(point);
  duration += timeBetweenPoints;
}

void Track::reset() {
  recPositions.clear();
  duration = std::chrono::milliseconds(0);
}

Duration Track::getDuration() const { return duration; }

const std::vector<Coordinates2d>& Track::getPositions() const {
  return recPositions;
}
