#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
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

  [[nodiscard]] std::vector<Segment> asSegments() const;

  [[nodiscard]] const std::vector<Coordinates2d>& getPositions() const;
  [[nodiscard]] Duration getDuration() const;

 private:
  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive& archive, const unsigned int /*version*/) {
    archive& duration;
    archive& timeBetweenPoints;
    archive& recPositions;
  }

  Duration duration;
  Duration timeBetweenPoints;
  std::vector<Coordinates2d> recPositions;
};
