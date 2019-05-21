#ifndef SIMPLESPACEOBJECT_H
#define SIMPLESPACEOBJECT_H
#include <vector>
#include "coordinates.h"
#include "segment.h"

class SimpleSpaceObject {
 public:
  SimpleSpaceObject(Coordinates crd = Coordinates(), unsigned int height = 0,
                    bool focus = false)
      : crds(crd), height(height), focus(focus) {}

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

  std::vector<Coordinates2d> getIntersectionPts(const Segment &segm) const {
    std::vector<Coordinates2d> inters;
    std::vector<Segment> walls;
    Coordinates2d tl, tr, bl, br;
    tl = tr = bl = br = crds;
    tl.x -= 0.5f;
    tl.y -= 0.5f;
    tr.x += 0.5f;
    tr.y -= 0.5f;
    bl.x -= 0.5f;
    bl.y += 0.5f;
    br.x += 0.5f;
    br.y += 0.5f;
    walls.push_back(Segment(tl, tr));
    walls.push_back(Segment(tr, br));
    walls.push_back(Segment(br, bl));
    walls.push_back(Segment(bl, tl));

    for (const auto &wall : walls) {
      if (wall.intersects(segm)) {
        inters.push_back(wall.getIntersectionPoint(segm));
      }
    }
    return inters;
  }

  Coordinates crds;
  unsigned int height;
  bool focus;
};

#endif  // SIMPLESPACEOBJECT_H
