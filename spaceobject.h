#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include "coordinates.h"

class SpaceObject {
 public:
  SpaceObject(Coordinates lb, Coordinates lt, Coordinates rt, Coordinates rb)
      : lb(lb), lt(lt), rt(rt), rb(rb) {}

  Coordinates getCenter() {
    return Coordinates((lb.x + lt.x + rt.x + rb.x) / 4.0f,
                       (lb.y + lt.y + rt.y + rb.y) / 4.0f,
                       (lb.z + lt.z + rt.z + rb.z) / 4.0f);
  }
  float getDistanceFromCenter();
  float getWHRatio();
  static float distance3d(Coordinates a, Coordinates b);

 private:
  Coordinates lb, lt, rt, rb;
};

#endif  // SPACEOBJECT_H
