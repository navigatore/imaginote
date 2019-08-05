#include "recordinginfo.h"

RecordingInfo::RecordingInfo() {}

void RecordingInfo::setSpaceName(const std::string spaceName) {
  this->spaceName = spaceName;
}

void RecordingInfo::setSpace(const Space &space) { this->space = space; }

void RecordingInfo::setSonificationMethodName(
    const std::string &sonificationMethodName) {
  this->sonificationMethodName = sonificationMethodName;
}

void RecordingInfo::setAngleX(const Angle &angleX) { this->angleX = angleX; }

void RecordingInfo::setMaxDistance(float maxDistance) {
  this->maxDistance = maxDistance;
}

std::string RecordingInfo::getSpaceName() const {
  fullySetUpTest();
  return *spaceName;
}

Space RecordingInfo::getSpace() const {
  fullySetUpTest();
  return *space;
}

std::string RecordingInfo::getSonificationMethodName() const {
  fullySetUpTest();
  return *sonificationMethodName;
}

Angle RecordingInfo::getAngleX() const {
  fullySetUpTest();
  return *angleX;
}

float RecordingInfo::getMaxDistance() const {
  fullySetUpTest();
  return *maxDistance;
}

void RecordingInfo::fullySetUpTest() const {
  if (!fullySetUp()) {
    throw NotFullySetUp();
  }
}

bool RecordingInfo::fullySetUp() const noexcept {
  return spaceName && space && sonificationMethodName && angleX && maxDistance;
}
