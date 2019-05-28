#ifndef TRACK_H
#define TRACK_H

#include <chrono>
#include <fstream>
#include <vector>
#include "coordinates2d.h"

class Track {
 public:
  Track(std::chrono::milliseconds timeBetweenPoints);

  void addPosition(const Coordinates2d& point);
  void reset();
  void save(std::ofstream& f);
  void load(std::ifstream& f);

  [[nodiscard]] const std::vector<Coordinates2d>& getPositions() const;

 private:
  std::chrono::microseconds duration;
  std::chrono::microseconds timeBetweenPoints;
  std::vector<Coordinates2d> recPositions;
};

#endif  // TRACK_H
