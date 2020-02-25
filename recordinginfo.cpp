#include "recordinginfo.h"

RecordingInfo::RecordingInfo() {}

void RecordingInfo::setSpaceName(const std::string &spaceName_) {
  spaceName = spaceName_;
}

void RecordingInfo::setSpace(const Space &space_) { space = space_; }

void RecordingInfo::setSonificationMethodName(
    const std::string &sonificationMethodName_) {
  sonificationMethodName = sonificationMethodName_;
}

void RecordingInfo::setAngleX(const Angle &angleX_) { angleX = angleX_; }

void RecordingInfo::setMaxDistance(float maxDistance_) {
  maxDistance = maxDistance_;
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
