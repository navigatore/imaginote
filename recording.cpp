#include "recording.h"
#include <boost/archive/text_oarchive.hpp>
#include <fstream>

Recording::Recording(const RecordingInfo &recordingInfo)
    : space(recordingInfo.getSpace()),
      sonificationMethodName(recordingInfo.getSonificationMethodName()),
      angleX(recordingInfo.getAngleX()),
      maxDistance(recordingInfo.getMaxDistance()) {}

Recording::Recording(const std::string &filename, bool oldVersion)
    : oldVersion(oldVersion) {
  if (!oldVersion) {
    loadRecording(filename);
  } else {
    loadOldRecording(filename);
  }
}

void Recording::saveRecording(const std::string &filename) {
  std::ofstream f(filename.c_str());
  boost::archive::text_oarchive oa(f);
  oa << recordingMagicNumber << recordingVersion2Constant << space << track
     << _exitReached << sonificationMethodName << angleX << maxDistance;
}

void Recording::exitReached() { _exitReached = true; }

void Recording::addPosition(const Coordinates2d &position) {
  track.addPosition(position);
}

bool Recording::getExitReached() const noexcept { return _exitReached; }

std::string Recording::getSonificationMethodName() const noexcept {
  return sonificationMethodName;
}

Angle Recording::getVisualAngle() const noexcept { return angleX; }

float Recording::getDistanceLimit() const noexcept { return maxDistance; }

const Space &Recording::getSpace() const noexcept { return space; }

const Track &Recording::getTrack() const noexcept { return track; }

void Recording::loadRecording(const std::string &filename) {
  std::ifstream f;
  f.open(filename.c_str());
  boost::archive::text_iarchive ia(f);
  uint32_t magicNumber{}, version{};
  ia >> magicNumber >> version;
  if (magicNumber != recordingMagicNumber ||
      version != recordingVersion2Constant) {
    throw InvalidFile();
  }
  ia >> space >> track >> _exitReached >> sonificationMethodName >> angleX >>
      maxDistance;
}

void Recording::loadOldRecording(const std::string &filename) {
  std::ifstream f;
  f.open(filename.c_str());
  boost::archive::text_iarchive ia(f);
  ia >> space >> track;
}
