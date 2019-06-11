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

void SimpleSpaceObject::setCorners(const SimpleSpaceObject *top,
                                   const SimpleSpaceObject *right,
                                   const SimpleSpaceObject *bottom,
                                   const SimpleSpaceObject *left) {
  if (_height == 0) {
    return;
  }

  bool topB = top != nullptr && top->height() > 0;
  bool rightB = right != nullptr && right->height() > 0;
  bool bottomB = bottom != nullptr && bottom->height() > 0;
  bool leftB = left != nullptr && left->height() > 0;

  corners = std::vector<Coordinates>();

  if (!topB && !rightB) {
    corners->push_back(Coordinates(_crds.x() + 0.5F + 0.01F, _crds.y(),
                                   _crds.z() - 0.5F - 0.01F));
  }
  if (!rightB && !bottomB) {
    corners->push_back(Coordinates(_crds.x() + 0.5F + 0.01F, _crds.y(),
                                   _crds.z() + 0.5F + 0.01F));
  }
  if (!bottomB && !leftB) {
    corners->push_back(Coordinates(_crds.x() - 0.5F - 0.01F, _crds.y(),
                                   _crds.z() + 0.5F + 0.01F));
  }
  if (!leftB && !topB) {
    corners->push_back(Coordinates(_crds.x() - 0.5F - 0.01F, _crds.y(),
                                   _crds.z() - 0.5F - 0.01F));
  }
}

std::vector<Coordinates> SimpleSpaceObject::getCorners() {
  if (!corners) {
    throw CornersNotSetUp();
  }
  return *corners;
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
  Coordinates2d tl, tr, bl, br, l, t, r, b;
  tl = tr = bl = br = l = t = r = b = _crds;
  tl.x() -= 0.5f;
  tl.y() -= 0.5f;
  tr.x() += 0.5f;
  tr.y() -= 0.5f;
  bl.x() -= 0.5f;
  bl.y() += 0.5f;
  br.x() += 0.5f;
  br.y() += 0.5f;
  l.x() -= 0.5f;
  r.x() += 0.5f;
  t.y() -= 0.5f;
  b.y() += 0.5f;
  walls.push_back(Segment(tl, tr));
  walls.push_back(Segment(tr, br));
  walls.push_back(Segment(br, bl));
  walls.push_back(Segment(bl, tl));
  //  walls.push_back(Segment(l, r));
  //  walls.push_back(Segment(t, b));

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

const bool &SimpleSpaceObject::visited() const { return _visited; }

bool &SimpleSpaceObject::visited() { return _visited; }

const bool &SimpleSpaceObject::focus() const { return _focus; }

const unsigned int &SimpleSpaceObject::height() const { return _height; }

const Coordinates &SimpleSpaceObject::crds() const { return _crds; }
