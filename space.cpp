#include "space.h"
#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>

Space::Space(const std::chrono::milliseconds &updatePeriod)
    : recTrack(updatePeriod) {}

void Space::loadFromFile(const char *fname) {
  clearState();
  std::ifstream f;
  f.open(fname);

  unsigned int height{};
  unsigned int width{};

  f >> name >> width >> height;
  this->height = height;
  this->width = width;

  if (!f) {
    throw InvalidSpaceFile();
  }

  for (unsigned int z = 0; z < height; ++z) {
    fields.emplace_back();
    for (unsigned int x = 0; x < width; ++x) {
      unsigned int height = 0;
      std::string tmp;
      f >> tmp;
      if (tmp == "x") {
        startPos = Coordinates(static_cast<float>(x), 0, static_cast<float>(z));
      } else {
        height = static_cast<unsigned int>(std::stoi(tmp));
      }
      fields[z].emplace_back(
          Coordinates(static_cast<float>(x), 0, static_cast<float>(z)), height,
          height > 0);
    }
  }

  f.close();
}

void Space::saveRecording(const std::string &filename) {
  std::ofstream f(filename.c_str(), std::ios::trunc | std::ios::binary);
  auto fixedWidth = static_cast<uint32_t>(width);
  f.write(reinterpret_cast<char *>(&fixedWidth), sizeof(fixedWidth));
  auto fixedHeight = static_cast<uint32_t>(height);
  f.write(reinterpret_cast<char *>(&fixedHeight), sizeof(fixedHeight));
  for (auto &row : fields) {
    for (auto &field : row) {
      auto fixedFieldHeight = static_cast<uint32_t>(field.height());
      f.write(reinterpret_cast<char *>(&fixedFieldHeight),
              sizeof(fixedFieldHeight));
    }
  }
  recTrack.save(f);
  f.close();
}

std::string Space::getName() { return name; }

void Space::setMapWidget(MapWidget *mapWidget) {
  this->mapWidget = mapWidget;
  mapWidget->setTrack(recTrack);
}

void Space::rotateListenerLeft(float time) { cone->rotateLeft(time); }

void Space::rotateListenerRight(float time) { cone->rotateRight(time); }

void Space::goForward(float time) {
  auto futurePosition = cone->tryForward(time);
  if (canGoInto(futurePosition)) {
    cone->forward(time);
  }
  updateStandingField();
}

void Space::goBackward(float time) {
  auto futurePosition = cone->tryBackward(time);
  if (canGoInto(futurePosition)) {
    cone->backward(time);
  }
  updateStandingField();
}

void Space::startPlaying(Angle angleX, float maxDistance,
                         GenericSpacePlayer *sp) {
  cone = ViewingCone(startPos, angleX, maxDistance);
  mapWidget->loadMap(fields);
  mapWidget->setAngleX(angleX / 2);
  mapWidget->setDistanceLimit(maxDistance);
  this->sp = sp;
  update(0);
}

void Space::stopPlaying() {
  delete sp;
  sp = nullptr;
  mapWidget->unloadMap();
  setFromBeginning();
}

void Space::update(float time) {
  if (recording) {
    recTrack.addPosition(cone->getPosition());
  }
  if (movingFocusAngle) {
    moveFocusAngle(time);
  }
  setFieldsFocus();
  updateClosestFocusField();
  mapWidget->update(cone->getPosition(), cone->getDirection(),
                    cone->getFocusAngle(), closestField);
  playClosestFocusField();
  sp->updateTime(time);
  sp->updateListenerPosition(cone->getPosition(), cone->getDirection());
}

std::string Space::listenerPositionStr() { return cone->getPosition().str(); }

std::string Space::listenerDirectionStr() { return cone->getDirection().str(); }

void Space::toggleMovingFocusAngle() {
  if (movingFocusAngle) {
    movingFocusAngle = false;
    cone->resetFocusAngle();
  } else {
    movingFocusAngle = true;
  }
}

void Space::toggleMapWidgetVisibility() {
  mapWidget->setVisible(!mapWidget->isVisible());
}

void Space::volumeUp() { sp->volumeUp(); }

void Space::volumeDown() { sp->volumeDown(); }

void Space::setRecording(bool activated) { recording = activated; }

bool Space::outOfMap() const {
  auto pos = getPlayerPosition();
  auto width = fields[0].size();
  auto height = fields.size();
  return pos.x() < -halfFieldSize || pos.x() > width - halfFieldSize ||
         pos.z() < -halfFieldSize || pos.z() > height - halfFieldSize;
}

bool Space::recordingEnabled() const { return recording; }

void Space::setFieldsFocus() {
  for (auto &row : fields) {
    for (auto &field : row) {
      if (cone->lookingAt(field)) {
        field.focus() = true;
      } else {
        field.focus() = false;
      }
    }
  }
}

void Space::updateClosestFocusField() {
  auto oldClosest = closestField;
  closestField.reset();

  for (const auto &row : fields) {
    for (auto field : row) {
      if (!closestField) {
        if (field.focus()) {
          closestField = field;
        }
      } else {
        if (field.focus() && firstCloser(field, *closestField)) {
          closestField = field;
        }
      }
    }
  }
  closestFieldChanged =
      ((closestField && !oldClosest) || (!closestField && oldClosest) ||
       (*oldClosest != *closestField));
}

void Space::updateStandingField() {
  auto x = static_cast<int>(std::round(cone->getPosition().x()));
  auto y = static_cast<int>(std::round(cone->getPosition().z()));
  auto ux = static_cast<unsigned int>(x);
  auto uy = static_cast<unsigned int>(y);

  if (x < 0 || y < 0 || uy >= fields.size() || ux >= fields[0].size()) {
    standingField = nullptr;
    return;
  }
  auto &newField = fields[uy][ux];
  sp->setStandingField(newField);
  if (standingField == nullptr) {
    standingField = &newField;
  } else if (newField != *standingField) {
    standingField->visited() = true;
    standingField = &newField;
  }
}

void Space::playClosestFocusField() {
  if (sp != nullptr) {
    if (closestField) {
      if (closestFieldChanged) {
        sp->setSonificationObject(*closestField);
      }
      sp->updateSonifiedPointPosition(
          cone->getFocusPointPosition(*closestField));
      sp->startPlaying();
    } else {
      sp->stopPlaying();
    }
  }
}

void Space::clearState() {
  fields.clear();
  cone.reset();
  movingFocusAngle = false;
  width = height = 0;
  setFromBeginning();
}

void Space::setFromBeginning() {
  cone.reset();
  closestField.reset();
  closestFieldChanged = false;
  standingField = nullptr;

  for (auto &row : fields) {
    for (auto &field : row) {
      field.visited() = false;
    }
  }

  recTrack.reset();
}

bool Space::canGoInto(const Coordinates &point) const {
  for (const auto &row : fields) {
    for (const auto &field : row) {
      if (field.height() > 0 && field.isInside(point)) {
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
  auto x_diff = obj.crds().x() - cone->getPosition().x();
  auto z_diff = obj.crds().z() - cone->getPosition().z();
  return x_diff * x_diff + z_diff * z_diff;
}

std::vector<std::vector<SimpleSpaceObject>> &Space::getFields() {
  return fields;
}

Coordinates Space::getPlayerPosition() const { return cone->getPosition(); }

float Space::getVolume() const { return sp->getVolume(); }

void Space::moveFocusAngle(float time) { cone->moveFocusAngle(time); }
