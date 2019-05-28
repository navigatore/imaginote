#include "track.h"

Track::Track(std::chrono::milliseconds timeBetweenPoints)
    : timeBetweenPoints(timeBetweenPoints) {}

void Track::addPosition(const Coordinates2d& point) {
  recPositions.push_back(point);
  duration += timeBetweenPoints;
}

void Track::reset() {
  recPositions.clear();
  duration = std::chrono::milliseconds(0);
}

void Track::save(std::ofstream& f) {
  auto fixedNoOfElements = static_cast<uint32_t>(recPositions.size());
  f.write(reinterpret_cast<char*>(&fixedNoOfElements),
          sizeof(fixedNoOfElements));
  for (auto& pos : recPositions) {
    f.write(reinterpret_cast<char*>(&pos.x()), sizeof(pos.x)());
    f.write(reinterpret_cast<char*>(&pos.y()), sizeof(pos.y)());
  }
}

void Track::load(std::ifstream& f) {
  recPositions.clear();
  uint32_t fixedNoOfElements = 0;
  f.read(reinterpret_cast<char*>(&fixedNoOfElements),
         sizeof(fixedNoOfElements));
  unsigned int noOfElements = fixedNoOfElements;
  for (unsigned int i = 0; i < noOfElements; ++i) {
    float x = 0, y = 0;
    f.read(reinterpret_cast<char*>(&x), sizeof(x));
    f.read(reinterpret_cast<char*>(&y), sizeof(y));
    recPositions.push_back(Coordinates2d(x, y));
  }
}

const std::vector<Coordinates2d>& Track::getPositions() const {
  return recPositions;
}
