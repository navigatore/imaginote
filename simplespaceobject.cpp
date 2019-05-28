#include "simplespaceobject.h"

SimpleSpaceObject::SimpleSpaceObject(Coordinates crd, unsigned int height,
                                     bool focus)
    : _crds(crd), _height(height), _focus(focus) {}

bool SimpleSpaceObject::operator==(const SimpleSpaceObject &other) {
  return _crds == other._crds && _height == other._height;
}

bool SimpleSpaceObject::operator!=(const SimpleSpaceObject &other) {
  return !(*this == other);
}

bool SimpleSpaceObject::isInside(const Coordinates &point) const {
  if (std::abs(point.x() - _crds.x()) < 0.5f &&
      std::abs(point.z() - _crds.z()) < 0.5f) {
    return true;
  }
  return false;
}

std::vector<Coordinates2d> SimpleSpaceObject::getIntersectionPts(
    const Segment &segm) const {
  std::vector<Coordinates2d> inters;
  std::vector<Segment> walls;
  Coordinates2d tl, tr, bl, br;
  tl = tr = bl = br = _crds;
  tl.x() -= 0.5f;
  tl.y() -= 0.5f;
  tr.x() += 0.5f;
  tr.y() -= 0.5f;
  bl.x() -= 0.5f;
  bl.y() += 0.5f;
  br.x() += 0.5f;
  br.y() += 0.5f;
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

Coordinates &SimpleSpaceObject::crds() { return _crds; }

unsigned int &SimpleSpaceObject::height() { return _height; }

bool &SimpleSpaceObject::focus() { return _focus; }

const bool &SimpleSpaceObject::focus() const { return _focus; }

const unsigned int &SimpleSpaceObject::height() const { return _height; }

const Coordinates &SimpleSpaceObject::crds() const { return _crds; }
