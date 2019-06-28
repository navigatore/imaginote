#pragma once
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include <vector>
#include "coordinates.h"
#include "simplespaceobject.h"

class Space {
 public:
  static constexpr auto fieldSize = 1.0F;

  void loadFromTextFile(std::ifstream &f);
  void saveToTextFile(std::ofstream &f);
  void createEmptySpace(unsigned int width, unsigned int height);
  void findCorners();
  void reset();
  void setFieldHeight(unsigned int x, unsigned int z, unsigned int height);
  void setStartPosition(const Coordinates &start);

  [[nodiscard]] bool isLoaded();
  [[nodiscard]] bool hasFieldBetweenPoints(
      const Coordinates &firstPoint, const Coordinates &secondPoint) const;

  [[nodiscard]] const std::vector<std::vector<SimpleSpaceObject>> &getFields()
      const;
  [[nodiscard]] std::vector<std::vector<SimpleSpaceObject>> &getFields();
  [[nodiscard]] unsigned int getFieldsWidth() const;
  [[nodiscard]] unsigned int getFieldsHeight() const;
  [[nodiscard]] std::vector<Coordinates2d> getInnerCorners() const;
  [[nodiscard]] std::vector<Coordinates2d> getExitCorners() const;
  [[nodiscard]] Coordinates getStartPosition() const;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int /*version*/) {
    ar &fields;
    ar &corners;
    ar &exitCorners;
    ar &startPosition;
    ar &loaded;
  }

  void findAllCorners();
  void findExitCorners();
  void removeFalseCorners();

  [[nodiscard]] std::size_t getNoOfFieldsAxisX() const;
  [[nodiscard]] std::size_t getNoOfFieldsAxisZ() const;

  std::vector<std::vector<SimpleSpaceObject>> fields;
  std::vector<Coordinates2d> corners;
  std::vector<Coordinates2d> exitCorners;
  Coordinates startPosition;
  bool loaded{};
};
