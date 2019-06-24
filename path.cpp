#include "path.h"
#include <exception>
#include <optional>

void Path::addNode(const Coordinates2d &point) {
  nodes.push_back(point);
  if (nodes.size() > 1) {
    segments.emplace_back(*(std::end(nodes) - 2), *(std::end(nodes) - 1));
  }
}

float Path::distance(const Coordinates2d &point) const {
  if (segments.size() == 0) {
    throw std::logic_error(
        "Path must contain at least two points to calculate distance from "
        "a point");
  }
  std::optional<float> minDistance;
  for (const auto &segment : segments) {
    auto currentDistance = segment.distanceFromPoint(point);
    if (!minDistance || *minDistance > currentDistance) {
      minDistance = currentDistance;
    }
  }
  return *minDistance;
}

const std::vector<Coordinates2d> &Path::getNodes() const { return nodes; }
