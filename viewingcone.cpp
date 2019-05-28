#include "viewingcone.h"
#include <stdexcept>

ViewingCone::ViewingCone(Angle viewAngleX, Angle viewAngleY, float maxDistance)
    : direction(90),
      focusAngle(90),
      viewAngleX(viewAngleX),
      viewAngleY(viewAngleY),
      maxDistance(maxDistance),
      moveSpeed(1.0f),
      rotateSpeed(45.0f),
      focusAngleMoveSpeed(15.0f) {}

void ViewingCone::forward(float time) { position = tryForward(time); }

void ViewingCone::backward(float time) { position = tryBackward(time); }

void ViewingCone::moveFocusAngle(float time) {
  focusAngle += focusAngleMoveSpeed * time;
  if (!focusAngle.inRange(direction - viewAngleX / 2,
                          direction + viewAngleX / 2)) {
    focusAngle = direction - viewAngleX / 2;
  }
}

void ViewingCone::resetFocusAngle() { focusAngle = direction; }

void ViewingCone::resetDirection() {
  direction = Angle(90.0F);
  resetFocusAngle();
}

Coordinates ViewingCone::getPosition() const { return position; }

Coordinates2d ViewingCone::getFocusPointPosition(
    const SimpleSpaceObject &obj) const {
  if (!lookingAt(obj)) {
    throw std::runtime_error(
        "Cannot get focus point position from an object which is not in focus");
  }
  auto intersectionPts(obj.getIntersectionPts(getFocusSegment()));
  return closestIntersectionPoint(intersectionPts);
}

Angle ViewingCone::getDirection() { return direction; }

Angle ViewingCone::getFocusAngle() const { return focusAngle; }

Coordinates ViewingCone::tryForward(float time) {
  Coordinates newPosition = position;
  newPosition.x() += moveSpeed * std::cos(direction.getRad()) * time;
  newPosition.z() -= moveSpeed * std::sin(direction.getRad()) * time;
  return newPosition;
}

Coordinates ViewingCone::tryBackward(float time) { return tryForward(-time); }

Angle ViewingCone::getAngleX() { return viewAngleX; }

void ViewingCone::setPosition(Coordinates position) {
  this->position = position;
}

void ViewingCone::setAngleX(const Angle &angleX) { viewAngleX = angleX; }

void ViewingCone::setDistanceLimit(float limit) { maxDistance = limit; }

void ViewingCone::rotateLeft(float time) {
  Angle change(rotateSpeed * time);
  direction += change;
  focusAngle += change;
}

void ViewingCone::rotateRight(float time) { rotateLeft(-time); }

bool ViewingCone::isInside(Coordinates point) {
  if (position.distance2d(point) > maxDistance) {
    return false;
  }

  auto rightDirection = direction;
  auto leftDirection = direction;
  rightDirection -= viewAngleX.getDeg() / 2.0f;
  leftDirection += viewAngleX.getDeg() / 2.0f;

  auto rightNormal = planeNormalFromAngle(rightDirection);
  auto leftNormal = -planeNormalFromAngle(leftDirection);

  if (viewAngleX.getDeg() < 180.0f)
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

bool ViewingCone::lookingAt(const SimpleSpaceObject &obj) const {
  if (obj.height() == 0) {
    return false;
  }
  auto intersectionPts = obj.getIntersectionPts(getFocusSegment());
  if (intersectionPts.size() == 0) {
    return false;
  }
  return Coordinates2d(position).distance(
             closestIntersectionPoint(intersectionPts)) < maxDistance;
}

bool ViewingCone::planeInequalityTest(Coordinates tested,
                                      Coordinates planeNormal,
                                      Coordinates pointOnPlane) {
  auto a = planeNormal.x(), b = planeNormal.y(), c = planeNormal.z();
  auto d = -(planeNormal.x() * pointOnPlane.x() +
             planeNormal.y() * pointOnPlane.y() +
             planeNormal.z() * pointOnPlane.z());

  return a * tested.x() + b * tested.y() + c * tested.z() + d < 0.0f;
}

Coordinates ViewingCone::planeNormalFromAngle(Angle angle) {
  return Coordinates(std::sin(angle.getRad()), 0.0f, std::cos(angle.getRad()));
}

Line ViewingCone::getFocusLine() const {
  Coordinates2d other_pos = position;
  if (focusAngle == Angle(90.0f) || focusAngle == Angle(270.0f)) {
    other_pos.y() += 1.0f;
  } else {
    other_pos.x() += std::cos(focusAngle.getRad());
    other_pos.y() -= std::sin(focusAngle.getRad());
  }
  return Line(position, other_pos);
}

Segment ViewingCone::getFocusSegment() const {
  Coordinates2d endPos = position;
  endPos.x() += maxDistance * std::cos(focusAngle.getRad());
  endPos.y() -= maxDistance * std::sin(focusAngle.getRad());
  return Segment(position, endPos);
}

Coordinates2d ViewingCone::closestIntersectionPoint(
    const std::vector<Coordinates2d> &interPoints) const {
  if (interPoints.size() == 0) {
    throw std::runtime_error(
        "Cannot return closest point if there is no point in the vector");
  }
  auto min_dist = Coordinates2d(position).distance(interPoints[0]);
  Coordinates2d closest(interPoints[0]);
  for (const auto &pt : interPoints) {
    auto current_distance(Coordinates2d(position).distance(pt));
    if (current_distance < min_dist) {
      closest = pt;
      min_dist = current_distance;
    }
  }
  return closest;
}
