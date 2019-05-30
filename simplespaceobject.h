#ifndef SIMPLESPACEOBJECT_H
#define SIMPLESPACEOBJECT_H
#include <vector>
#include "coordinates.h"
#include "segment.h"

class SimpleSpaceObject {
 public:
  SimpleSpaceObject(Coordinates crd = Coordinates(), unsigned int height = 0,
                    bool focus = false);

  bool operator==(const SimpleSpaceObject& other);
  bool operator!=(const SimpleSpaceObject& other);

  [[nodiscard]] bool isInside(const Coordinates& point) const;
  [[nodiscard]] std::vector<Coordinates2d> getIntersectionPts(
      const Segment& segm) const;

  [[nodiscard]] const Coordinates& crds() const;
  Coordinates& crds();
  [[nodiscard]] const unsigned int& height() const;
  unsigned int& height();
  [[nodiscard]] const bool& focus() const;
  bool& focus();
  [[nodiscard]] const bool& visited() const;
  bool& visited();

 private:
  Coordinates _crds;
  unsigned int _height;
  bool _focus;
  bool _visited{};
};

#endif  // SIMPLESPACEOBJECT_H
