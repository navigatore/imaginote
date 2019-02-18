#ifndef SIMPLESPACEOBJECT_H
#define SIMPLESPACEOBJECT_H
#include "coordinates.h"

class SimpleSpaceObject {
 public:
  SimpleSpaceObject(Coordinates crd = Coordinates(), unsigned int height = 0,
                    bool visible = false, bool focus = false)
      : crds(crd), height(height), visible(visible), focus(focus) {}

  bool operator==(const SimpleSpaceObject &other) {
    return crds == other.crds && height == other.height;
  }

  bool operator!=(const SimpleSpaceObject &other) { return !(*this == other); }

  bool isInside(const Coordinates &point) const {
    if (std::abs(point.x - crds.x) < 0.5f &&
        std::abs(point.z - crds.z) < 0.5f) {
      return true;
    }
    return false;
  }

  Coordinates crds;
  unsigned int height;
  bool visible, focus;
};

#endif  // SIMPLESPACEOBJECT_H
