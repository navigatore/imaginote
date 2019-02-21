#include "space.h"
#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>

Space::Space(int updateFreq)
    : updateFreq(updateFreq),
      cone(Angle(90.0f), Angle(180.0f), Angle(180.0f), 1000.0f),
      closestField(0.0f, 0.0f, 0.0f),
      sp(nullptr),
      closestFieldExists(false),
      closestFieldChanged(false),
      movingFocusAngle(false),
      mapWidget(nullptr) {}

void Space::loadFromFile(const char *fname) {
  clearState();
  std::ifstream f;
  f.open(fname);

  unsigned int height, width;

  f >> name >> height >> width;

  if (!f) throw InvalidSpaceFile();

  for (unsigned int z = 0; z < width; ++z) {
    fields.push_back(std::vector<SimpleSpaceObject>());
    for (unsigned int x = 0; x < height; ++x) {
      unsigned int height = 0;
      std::string tmp;
      f >> tmp;
      if (tmp == "x") {
        cone.setPosition(Coordinates(x, 0, z));
      } else {
        height = static_cast<unsigned int>(std::stoi(tmp));
      }
      auto obj = SimpleSpaceObject(Coordinates(x, 0, z), height, height > 0);
      fields[z].push_back(obj);
    }
  }

  // TODO: Check, if EOF

  f.close();

  update(0);
}

std::string Space::getName() { return name; }

void Space::setMapWidget(MapWidget *mapWidget) { this->mapWidget = mapWidget; }

void Space::rotateListenerLeft(float time) { cone.rotateLeft(time); }

void Space::rotateListenerRight(float time) { cone.rotateRight(time); }

void Space::goForward(float time) {
  auto futurePosition = cone.tryForward(time);
  if (canGoInto(futurePosition)) {
    cone.forward(time);
  }
}

void Space::goBackward(float time) {
  auto futurePosition = cone.tryBackward(time);
  if (canGoInto(futurePosition)) {
    cone.backward(time);
  }
}

void Space::startPlaying(Angle angleX, float maxDistance) {
  cone.setAngleX(angleX);
  cone.setDistanceLimit(maxDistance);
  mapWidget->loadMap(fields);
  mapWidget->setAngleX(angleX / 2);
  mapWidget->setDistanceLimit(maxDistance);
  sp = new NewSpacePlayer();
  sp->sonificateObject(closestField);
}

void Space::stopPlaying() {
  delete sp;
  sp = nullptr;
  mapWidget->unloadMap();
}

void Space::update(float time) {
  if (movingFocusAngle) {
    moveFocusAngle(time);
  }
  setFieldsFocus();
  updateClosestFocusField();
  auto closestFieldPtr = closestFieldExists ? &closestField : nullptr;
  mapWidget->update(cone.getPosition(), cone.getDirection(),
                    cone.getFocusAngle(), closestFieldPtr);
  playClosestFocusField();
  sp->updateListenerPosition(cone.getPosition(), cone.getDirection());
}

std::string Space::listenerPositionStr() { return cone.getPosition().str(); }

std::string Space::listenerDirectionStr() { return cone.getDirection().str(); }

void Space::toggleMovingFocusAngle() { movingFocusAngle = !movingFocusAngle; }

void Space::setFieldsFocus() {
  for (auto &row : fields) {
    for (auto &field : row) {
      if (cone.lookingAt(field)) {
        field.focus = true;
      } else {
        field.focus = false;
      }
    }
  }
}

void Space::updateClosestFocusField() {
  auto oldClosest = closestField;
  auto existed = closestFieldExists;
  closestField = SimpleSpaceObject(0.0f, 0.0f, 0.0f);
  closestFieldExists = false;

  for (auto row : fields) {
    for (auto field : row) {
      if (!closestFieldExists) {
        if (field.focus) {
          closestField = field;
          closestFieldExists = true;
        }
      } else {
        if (field.focus && firstCloser(field, closestField)) {
          closestField = field;
        }
      }
    }
  }
  closestFieldChanged =
      ((closestFieldExists && !existed) || (!closestFieldExists && existed) ||
       (oldClosest != closestField));
}

void Space::playClosestFocusField() {
  if (sp != nullptr && closestFieldChanged) {
    if (closestFieldExists) {
      sp->sonificateObject(closestField);
    } else {
      sp->stopPlaying();
    }
  }
}

void Space::clearState() {
  fields.clear();
  cone = ViewingCone(Angle(90.0f), Angle(180.0f), Angle(180.0f), 5.0f);
  focusAngleMoveSpeed = 30.0f;
  closestField = SimpleSpaceObject(0.0f, 0.0f, 0.0f);
  closestFieldExists = false;
  closestFieldChanged = false;
  movingFocusAngle = false;
}

bool Space::canGoInto(const Coordinates &point) const {
  for (const auto &row : fields) {
    for (const auto &field : row) {
      if (field.height > 0 && field.isInside(point)) {
        return false;
      }
    }
  }
  return true;
}

bool Space::firstCloser(const SimpleSpaceObject &first,
                        const SimpleSpaceObject &second) {
  return distanceSqFrom(first) < distanceSqFrom(second);
}

float Space::distanceSqFrom(SimpleSpaceObject obj) {
  auto x_diff = obj.crds.x - cone.getPosition().x;
  auto z_diff = obj.crds.z - cone.getPosition().z;
  return x_diff * x_diff + z_diff * z_diff;
}

std::vector<std::vector<SimpleSpaceObject> > &Space::getFields() {
  return fields;
}

Coordinates Space::getPlayerPosition() { return cone.getPosition(); }

void Space::moveFocusAngle(float time) { cone.moveFocusAngle(time); }
