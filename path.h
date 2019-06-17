#ifndef PATH_H
#define PATH_H

#include <vector>
#include "coordinates2d.h"
#include "segment.h"

class Path {
 public:
  void addNode(const Coordinates2d& point);

  [[nodiscard]] float distance(const Coordinates2d& point) const;
  [[nodiscard]] const std::vector<Coordinates2d>& getNodes() const;

 private:
  std::vector<Coordinates2d> nodes;
  std::vector<Segment> segments;
};

#endif  // PATH_H
