#ifndef TRACK_H
#define TRACK_H

#include <chrono>
#include <fstream>
#include <vector>
#include "coordinates2d.h"
#include "duration.h"

class Track {
 public:
  Track(Duration timeBetweenPoints);

  void addPosition(const Coordinates2d& point);
  void reset();
  void save(std::ofstream& f);
  void load(std::ifstream& f);

  [[nodiscard]] const std::vector<Coordinates2d>& getPositions() const;
  [[nodiscard]] Duration getDuration() const;

 private:
  Duration duration;
  Duration timeBetweenPoints;
  std::vector<Coordinates2d> recPositions;
};

#endif  // TRACK_H
