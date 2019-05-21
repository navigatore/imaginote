#ifndef TRACK_H
#define TRACK_H

#include <chrono>
#include <vector>
#include "coordinates2d.h"

class Track {
 public:
  Track(std::chrono::milliseconds timeBetweenPoints);

  void addPosition(const Coordinates2d& point);
  void reset();

  const std::vector<Coordinates2d>& getPositions() const;

 private:
  std::chrono::milliseconds duration;
  std::chrono::milliseconds timeBetweenPoints;
  std::vector<Coordinates2d> recPositions;
};

#endif  // TRACK_H
