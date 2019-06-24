#include "space.h"
#include <algorithm>

void Space::loadFromFile(std::ifstream &f) {
  fields.clear();
  corners.clear();
  exitCorners.clear();

  uint32_t fixedWidth = 0;
  f.read(reinterpret_cast<char *>(&fixedWidth), sizeof(fixedWidth));
  auto width = static_cast<unsigned int>(fixedWidth);
  uint32_t fixedHeight = 0;
  f.read(reinterpret_cast<char *>(&fixedHeight), sizeof(fixedHeight));
  auto height = static_cast<unsigned int>(fixedHeight);

  for (unsigned int z = 0; z < height; ++z) {
    fields.emplace_back();
    for (unsigned int x = 0; x < width; ++x) {
      uint32_t fixedFieldHeight = 0;
      f.read(reinterpret_cast<char *>(&fixedFieldHeight),
             sizeof(fixedFieldHeight));
      auto fieldHeight = static_cast<unsigned int>(fixedFieldHeight);
      auto floatX = static_cast<float>(x);
      auto floatZ = static_cast<float>(z);
      auto obj = SimpleSpaceObject(Coordinates(floatX, 0, floatZ), fieldHeight,
                                   fieldHeight > 0);
      fields[z].push_back(obj);
    }
  }

  findCorners();
}

void Space::createEmptySpace(unsigned int width, unsigned int height) {
  fields.clear();
  fields.resize(height);
  std::for_each(begin(fields), end(fields),
                [width](auto &row) { row.resize(width); });
  for (unsigned int z = 0; z < height; ++z) {
    for (unsigned int x = 0; x < width; ++x) {
      fields[z][x].crds() = Coordinates(x, 0, z);
    }
  }
}

bool Space::isLoaded() { return !fields.empty(); }

bool Space::hasFieldBetweenPoints(const Coordinates &firstPoint,
                                  const Coordinates &secondPoint) const {
  Segment straightBetweenTwoPoints(firstPoint, secondPoint);
  for (const auto &row : fields) {
    for (const auto &field : row) {
      if (field.height() == 0) {
        continue;
      }
      auto intersectionPoints =
          field.getIntersectionPts(straightBetweenTwoPoints);

      auto point{std::begin(intersectionPoints)};
      while (point != std::end(intersectionPoints)) {
        if (Coordinates(*point) == firstPoint ||
            Coordinates(*point) == secondPoint) {
          point = intersectionPoints.erase(point);
        } else {
          ++point;
        }
      }

      if (!intersectionPoints.empty()) {
        return true;
      }
    }
  }
  return false;
}

std::vector<std::vector<SimpleSpaceObject> > &Space::getFields() {
  return fields;
}

void Space::findAllCorners() {
  for (unsigned int y = 0; y < fields.size(); ++y) {
    for (unsigned int x = 0; x < fields[0].size(); ++x) {
      if (fields[y][x].height() > 0) {
        auto top = (y > 0) ? &fields[y - 1][x] : nullptr;
        auto right = (x < fields[0].size() - 1) ? &fields[y][x + 1] : nullptr;
        auto bottom = (y < fields.size() - 1) ? &fields[y + 1][x] : nullptr;
        auto left = (x > 0) ? &fields[y][x - 1] : nullptr;
        fields[y][x].setCorners(top, right, bottom, left);
        auto newCorners = fields[y][x].getCorners();
        corners.insert(std::end(corners), std::begin(newCorners),
                       std::end(newCorners));
      }
    }
  }
}

void Space::findExitCorners() {
  auto xBound = getFieldsWidth() - fieldSize;
  auto yBound = getFieldsHeight() - fieldSize;
  auto current = std::begin(corners);
  while (current != std::end(corners)) {
    if (current->x() < 0 || current->y() < 0 || current->x() > xBound ||
        current->y() > yBound) {
      exitCorners.push_back(*current);
      current = corners.erase(current);
    } else {
      ++current;
    }
  }
}

void Space::removeFalseCorners() {
  auto xBound = getFieldsWidth() - fieldSize;
  auto yBound = getFieldsHeight() - fieldSize;
  auto current = std::begin(corners);
  while (current != std::end(corners)) {
    if ((current->x() < 0 && current->y() < 0) ||
        (current->x() > xBound && current->y() < 0) ||
        (current->x() > xBound && current->y() > yBound) ||
        (current->x() < 0 && current->y() > yBound)) {
      current = corners.erase(current);
    } else {
      ++current;
    }
  }
}

void Space::findCorners() {
  findAllCorners();
  removeFalseCorners();
  findExitCorners();
}

unsigned int Space::getFieldsWidth() {
  return static_cast<unsigned int>(fields[0].size());
}

unsigned int Space::getFieldsHeight() {
  return static_cast<unsigned int>(fields.size());
}

std::vector<Coordinates2d> Space::getInnerCorners() const { return corners; }

std::vector<Coordinates2d> Space::getExitCorners() const { return exitCorners; }

const std::vector<std::vector<SimpleSpaceObject> > &Space::getFields() const {
  return fields;
}
