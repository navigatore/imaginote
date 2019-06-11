#ifndef SPACE_H
#define SPACE_H

#include <fstream>
#include <vector>
#include "coordinates.h"
#include "simplespaceobject.h"

class Space {
 public:
  static constexpr auto fieldSize = 1.0F;

  void loadFromFile(std::ifstream &f);

  [[nodiscard]] bool isLoaded();
  [[nodiscard]] bool hasFieldBetweenPoints(
      const Coordinates &firstPoint, const Coordinates &secondPoint) const;

  [[nodiscard]] const std::vector<std::vector<SimpleSpaceObject>> &getFields()
      const;
  [[nodiscard]] unsigned int getFieldsWidth();
  [[nodiscard]] unsigned int getFieldsHeight();
  [[nodiscard]] std::vector<Coordinates2d> getInnerCorners() const;
  [[nodiscard]] std::vector<Coordinates2d> getExitCorners() const;

 private:
  void findCorners();
  void findAllCorners();
  void findExitCorners();
  void removeFalseCorners();

  std::vector<std::vector<SimpleSpaceObject>> fields;
  std::vector<Coordinates2d> corners;
  std::vector<Coordinates2d> exitCorners;
};

#endif  // SPACE_H
