#include "simulationcontroller.h"
#include <algorithm>
#include <exception>
#include <fstream>

void SimulationController::loadFromFile(const char *fname) {
  clearState();
  std::ifstream file;
  file.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
  try {
    file.open(fname);
    space.loadFromTextFile(file);
    file.close();
    startPos = space.getStartPosition();
    name = fname;
    name.removeDirectoryPath();
    name.removeExtension();
  } catch (...) {
    throw InvalidSpaceFile();
  }
}

bool SimulationController::getExitReached() const noexcept {
  return exitReached;
}

std::string SimulationController::getName() { return name; }

void SimulationController::setMapWidget(MapWidget *mapWidget) {
  this->mapWidget = mapWidget;
}

void SimulationController::rotateListenerLeft(float time) {
  cone->rotateLeft(time);
}

void SimulationController::rotateListenerRight(float time) {
  cone->rotateRight(time);
}

void SimulationController::goForward(float time) {
  auto futurePosition = cone->tryForward(time);
  if (canGoInto(futurePosition)) {
    cone->forward(time);
  }
  updateStandingField();
}

void SimulationController::goBackward(float time) {
  auto futurePosition = cone->tryBackward(time);
  if (canGoInto(futurePosition)) {
    cone->backward(time);
  }
  updateStandingField();
}

void SimulationController::startPlaying(Angle angleX, float maxDistance,
                                        GenericSpacePlayer *sp) {
  cone = ViewingCone(startPos, angleX, maxDistance);
  this->sp = sp;
  RecordingInfo recordingInfo;
  recordingInfo.setSpace(space);
  recordingInfo.setSonificationMethodName(sp->getName());
  recordingInfo.setAngleX(cone->getAngleX());
  recordingInfo.setMaxDistance(cone->getMaxDistance());
  recording = std::make_unique<Recording>(recordingInfo);

  mapWidget->loadMap(space.getFields());
  mapWidget->setAngleX(angleX / 2);
  mapWidget->setDistanceLimit(maxDistance);
  mapWidget->setTrack(recording->getTrack());

  update(0);
}

void SimulationController::stopPlaying() {
  delete sp;
  sp = nullptr;
  mapWidget->unloadMap();
  if (recordingEnabled) {
    saveRecording();
  }
  setFromBeginning();
}

void SimulationController::update(float time) {
  if (recordingEnabled) {
    recording->addPosition(cone->getPosition(), movingFocusAngle);
  }
  if (movingFocusAngle) {
    moveFocusAngle(time);
  }
  setFieldsFocus();
  updateClosestFocusField();
  mapWidget->update(cone->getPosition(), cone->getDirection(),
                    cone->getFocusAngle(), closestField);
  playClosestFocusField();
  if (outOfMap()) {
    exitReached = true;
    recording->exitReached();
  }
  sp->updateTime(time);
  sp->updateListenerPosition(cone->getPosition(), cone->getDirection());
}

std::string SimulationController::listenerPositionStr() {
  return cone->getPosition().str();
}

std::string SimulationController::listenerDirectionStr() {
  return cone->getDirection().str();
}

void SimulationController::toggleMovingFocusAngle() {
  if (movingFocusAngle) {
    movingFocusAngle = false;
    cone->resetFocusAngle();
  } else {
    movingFocusAngle = true;
  }
}

void SimulationController::toggleMapWidgetVisibility() {
  mapWidget->setVisible(!mapWidget->isVisible());
  recording->toggleMapPreview();
}

void SimulationController::volumeUp() { sp->volumeUp(); }

void SimulationController::volumeDown() { sp->volumeDown(); }

void SimulationController::setRecordingActivated(bool activated) {
  recordingEnabled = activated;
}

bool SimulationController::outOfMap() const noexcept {
  auto pos = getPlayerPosition();
  auto width = space.getFieldsWidth();
  auto height = space.getFieldsHeight();
  return pos.x() < -halfFieldSize || pos.x() > width - halfFieldSize ||
         pos.z() < -halfFieldSize || pos.z() > height - halfFieldSize;
}

bool SimulationController::getRecordingEnabled() const noexcept {
  return recordingEnabled;
}

void SimulationController::setFieldsFocus() {
  for (auto &row : space.getFields()) {
    for (auto &field : row) {
      if (cone->lookingAt(field)) {
        field.focus() = true;
      } else {
        field.focus() = false;
      }
    }
  }
}

void SimulationController::updateClosestFocusField() {
  auto oldClosest = closestField;
  closestField.reset();

  for (const auto &row : space.getFields()) {
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

void SimulationController::updateStandingField() {
  auto x = static_cast<int>(std::round(cone->getPosition().x()));
  auto y = static_cast<int>(std::round(cone->getPosition().z()));
  auto ux = static_cast<unsigned int>(x);
  auto uy = static_cast<unsigned int>(y);

  if (x < 0 || y < 0 || uy >= space.getFieldsHeight() ||
      ux >= space.getFieldsWidth()) {
    standingField = nullptr;
    return;
  }
  auto &newField = space.getFields()[uy][ux];
  sp->setStandingField(newField);
  if (standingField == nullptr) {
    standingField = &newField;
  } else if (newField != *standingField) {
    standingField->visited() = true;
    standingField = &newField;
  }
}

void SimulationController::playClosestFocusField() {
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

void SimulationController::saveRecording() {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  std::ostringstream filename;
  filename << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S") << ".rec";
  recording->saveRecording(std::string((filename.str())));
}

void SimulationController::clearState() {
  space.reset();
  cone.reset();
  movingFocusAngle = false;
  setFromBeginning();
}

void SimulationController::setFromBeginning() {
  cone.reset();
  closestField.reset();
  closestFieldChanged = false;
  standingField = nullptr;
  exitReached = false;

  for (auto &row : space.getFields()) {
    for (auto &field : row) {
      field.visited() = false;
    }
  }

  recording.release();
}

bool SimulationController::canGoInto(const Coordinates &point) const {
  for (const auto &row : space.getFields()) {
    for (const auto &field : row) {
      if (field.height() > 0 && field.isInside(point)) {
        return false;
      }
    }
  }
  return true;
}

bool SimulationController::firstCloser(const SimpleSpaceObject &first,
                                       const SimpleSpaceObject &second) {
  return distanceSqFrom(first) < distanceSqFrom(second);
}

float SimulationController::distanceSqFrom(SimpleSpaceObject obj) {
  auto x_diff = obj.crds().x() - cone->getPosition().x();
  auto z_diff = obj.crds().z() - cone->getPosition().z();
  return x_diff * x_diff + z_diff * z_diff;
}

std::vector<std::vector<SimpleSpaceObject>> &SimulationController::getFields() {
  return space.getFields();
}

Coordinates SimulationController::getPlayerPosition() const {
  return cone->getPosition();
}

float SimulationController::getVolume() const { return sp->getVolume(); }

void SimulationController::moveFocusAngle(float time) {
  cone->moveFocusAngle(time);
}
