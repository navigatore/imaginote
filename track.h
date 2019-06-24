#pragma once
#include <chrono>
#include <fstream>
#include <vector>
#include "coordinates2d.h"
#include "duration.h"
#include "segment.h"

class Track {
 public:
  Track(Duration timeBetweenPoints);

  void addPosition(const Coordinates2d& point);
  void reset();
  void save(std::ofstream& f);
  void load(std::ifstream& f);

  [[nodiscard]] std::vector<Segment> asSegments() const;

  [[nodiscard]] const std::vector<Coordinates2d>& getPositions() const;
  [[nodiscard]] Duration getDuration() const;

 private:
  Duration duration;
  Duration timeBetweenPoints;
  std::vector<Coordinates2d> recPositions;
};
