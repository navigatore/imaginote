#include "space.h"
#include <algorithm>

void Space::loadFromBinaryFile(std::ifstream &f) {
  reset();

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
  loaded = true;
}

void Space::saveToBinaryFile(std::ofstream &file) {
  auto fixedWidth = static_cast<uint32_t>(getNoOfFieldsAxisX());
  file.write(reinterpret_cast<char *>(&fixedWidth), sizeof(fixedWidth));
  auto fixedHeight = static_cast<uint32_t>(getNoOfFieldsAxisZ());
  file.write(reinterpret_cast<char *>(&fixedHeight), sizeof(fixedHeight));
  for (auto &row : fields) {
    for (auto &field : row) {
      auto fixedFieldHeight = static_cast<uint32_t>(field.height());
      file.write(reinterpret_cast<char *>(&fixedFieldHeight),
                 sizeof(fixedFieldHeight));
    }
  }
}

void Space::loadFromTextFile(std::ifstream &f) {
  reset();
  unsigned int height{};
  unsigned int width{};

  try {
    f >> width >> height;

    for (unsigned int z = 0; z < height; ++z) {
      fields.emplace_back();
      for (unsigned int x = 0; x < width; ++x) {
        unsigned int height = 0;
        std::string tmp;
        f >> tmp;
        if (tmp == "x") {
          startPosition =
              Coordinates(static_cast<float>(x), 0, static_cast<float>(z));
        } else {
          height = static_cast<unsigned int>(std::stoi(tmp));
        }
        fields[z].emplace_back(
            Coordinates(static_cast<float>(x), 0, static_cast<float>(z)),
            height, height > 0);
      }
    }
  } catch (std::ios_base::failure & /*e*/) {
    throw std::runtime_error("Invalid space file (text representation)");
  }

  loaded = true;
}

void Space::saveToTextFile(std::ofstream &file) {
  if (!loaded) {
    throw std::logic_error("Can't save space file if space is not loaded");
  }
  char separator = ' ';
  file << getNoOfFieldsAxisX() << separator << getNoOfFieldsAxisZ()
       << std::endl;
  for (const auto &row : fields) {
    for (const auto &field : row) {
      if (field.crds() != startPosition) {
        file << field.height();
      } else {
        file << 'x';
      }
      file << separator;
    }
    file << std::endl;
  }
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
  loaded = true;
}

bool Space::isLoaded() { return loaded; }

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

unsigned int Space::getFieldsWidth() const {
  return static_cast<unsigned int>(getNoOfFieldsAxisX());
}

unsigned int Space::getFieldsHeight() const {
  return static_cast<unsigned int>(getNoOfFieldsAxisZ());
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

std::size_t Space::getNoOfFieldsAxisX() const { return fields.at(0).size(); }

std::size_t Space::getNoOfFieldsAxisZ() const { return fields.size(); }

void Space::findCorners() {
  findAllCorners();
  removeFalseCorners();
  findExitCorners();
}

std::vector<Coordinates2d> Space::getInnerCorners() const { return corners; }

std::vector<Coordinates2d> Space::getExitCorners() const { return exitCorners; }

Coordinates Space::getStartPosition() const { return startPosition; }

void Space::reset() {
  loaded = false;
  fields.clear();
  corners.clear();
  exitCorners.clear();
}

void Space::setFieldHeight(unsigned int x, unsigned int z,
                           unsigned int height) {
  fields.at(z).at(x).height() = height;
}

void Space::setStartPosition(const Coordinates &start) {
  startPosition = start;
}

const std::vector<std::vector<SimpleSpaceObject> > &Space::getFields() const {
  return fields;
}
