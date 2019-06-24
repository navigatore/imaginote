#pragma once
#include "angle.h"
#include "coordinates.h"
#include "line.h"
#include "simplespaceobject.h"

class ViewingCone {
 public:
  ViewingCone(Coordinates startPosition, Angle viewAngleX, float maxDistance);

  void setPosition(Coordinates position);
  void setAngleX(const Angle &angleX);
  void setDistanceLimit(float limit);

  void rotateLeft(float time);
  void rotateRight(float time);
  void forward(float time);
  void backward(float time);
  void moveFocusAngle(float time);
  void resetFocusAngle();
  void resetDirection();

  [[nodiscard]] Coordinates getPosition() const;
  [[nodiscard]] Coordinates2d getFocusPointPosition(
      const SimpleSpaceObject &obj) const;
  Angle getAngleX();
  Angle getDirection();
  [[nodiscard]] Angle getFocusAngle() const;

  Coordinates tryForward(float time);
  Coordinates tryBackward(float time);

  bool isInside(Coordinates point);
  bool onLeftSide(Angle relativeAngle, Coordinates point);
  [[nodiscard]] bool lookingAt(const SimpleSpaceObject &obj) const;

 private:
  static bool planeInequalityTest(Coordinates tested, Coordinates planeNormal,
                                  Coordinates pointOnPlane);
  static Coordinates planeNormalFromAngle(Angle angle);
  [[nodiscard]] Line getFocusLine() const;
  [[nodiscard]] Segment getFocusSegment() const;
  [[nodiscard]] Coordinates2d closestIntersectionPoint(
      const std::vector<Coordinates2d> &interPoints) const;

  Coordinates position;
  Angle direction, focusAngle, viewAngleX;
  float maxDistance, moveSpeed, rotateSpeed, focusAngleMoveSpeed;
};
