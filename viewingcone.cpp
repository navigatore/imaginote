#include "viewingcone.h"

void ViewingCone::forward(float time) {
  position.x += std::cos(direction.getRad()) * time;
  position.z -= std::sin(direction.getRad()) * time;
}

void ViewingCone::backward(float time) {
  position.x -= std::cos(direction.getRad()) * time;
  position.z += std::sin(direction.getRad()) * time;
}

bool ViewingCone::isInside(Coordinates point) {
  auto rightDirection = direction;
  auto leftDirection = direction;
  rightDirection -= viewAngleX.value / 2.0f;
  leftDirection += viewAngleX.value / 2.0f;

  auto rightNormal = planeNormalFromAngle(rightDirection);
  auto leftNormal = -planeNormalFromAngle(leftDirection);

  if (viewAngleX.value < 180.0f)
    return planeInequalityTest(point, rightNormal, position) &&
           planeInequalityTest(point, leftNormal, position);
  return planeInequalityTest(point, rightNormal, position) ||
         planeInequalityTest(point, leftNormal, position);
}

bool ViewingCone::onLeftSide(Angle relativeAngle, Coordinates point) {
  auto angle = direction + relativeAngle;

  auto normal = planeNormalFromAngle(angle);

  return planeInequalityTest(point, normal, position);
}

bool ViewingCone::planeInequalityTest(Coordinates tested,
                                      Coordinates planeNormal,
                                      Coordinates pointOnPlane) {
  auto a = planeNormal.x, b = planeNormal.y, c = planeNormal.z;
  auto d = -(planeNormal.x * pointOnPlane.x + planeNormal.y * pointOnPlane.y +
             planeNormal.z * pointOnPlane.z);

  return a * tested.x + b * tested.y + c * tested.z + d < 0.0f;
}

Coordinates ViewingCone::planeNormalFromAngle(Angle angle) {
  return Coordinates(std::sin(angle.getRad()), 0.0f, std::cos(angle.getRad()));
}
