#ifndef VIEWINGCONE_H
#define VIEWINGCONE_H

#include "angle.h"
#include "coordinates.h"
#include "line.h"
#include "simplespaceobject.h"

class ViewingCone {
 public:
  ViewingCone(Angle direction, Angle viewAngleX, Angle viewAngleY,
              float maxDistance);

  void setPosition(Coordinates position);
  void setAngleX(const Angle &angleX);
  void setDistanceLimit(float limit);

  void rotateLeft(float time);
  void rotateRight(float time);
  void forward(float time);
  void backward(float time);
  void moveFocusAngle(float time);

  Coordinates getPosition();
  Angle getAngleX();
  Angle getDirection();
  Angle getFocusAngle() const;

  Coordinates tryForward(float time);
  Coordinates tryBackward(float time);

  bool isInside(Coordinates point);
  bool onLeftSide(Angle relativeAngle, Coordinates point);
  bool lookingAt(const SimpleSpaceObject &obj) const;

 private:
  static bool planeInequalityTest(Coordinates tested, Coordinates planeNormal,
                                  Coordinates pointOnPlane);
  static Coordinates planeNormalFromAngle(Angle angle);
  Line getFocusLine() const;
  Segment getFocusSegment() const;

  Coordinates position;
  Angle direction, focusAngle, viewAngleX, viewAngleY;
  float maxDistance, moveSpeed, rotateSpeed, focusAngleMoveSpeed;
};

#endif  // VIEWINGCONE_H
