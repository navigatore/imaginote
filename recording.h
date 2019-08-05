#pragma once
#include <cstdint>
#include <string>
#include "angle.h"
#include "recordinginfo.h"
#include "space.h"
#include "track.h"

class Recording {
 public:
  Recording(const RecordingInfo &recordingInfo);
  Recording(const std::string &filename, bool oldVersion = false);

  class InvalidFile : public std::exception {};

  void saveRecording(const std::string &filename);
  void exitReached();
  void addPosition(const Coordinates2d &position, bool movingFocusEnabled);
  void toggleMapPreview();

  [[nodiscard]] const Track &getTrack() const noexcept;
  [[nodiscard]] const Space &getSpace() const noexcept;
  [[nodiscard]] const std::vector<bool> &getMovingFocusVector() const noexcept;
  [[nodiscard]] const std::vector<bool> &getMapPreviewVector() const noexcept;
  [[nodiscard]] bool getExitReached() const noexcept;
  [[nodiscard]] std::string getSpaceName() const noexcept;
  [[nodiscard]] std::string getSonificationMethodName() const noexcept;
  [[nodiscard]] Angle getVisualAngle() const noexcept;
  [[nodiscard]] float getDistanceLimit() const noexcept;

 private:
  static constexpr uint32_t recordingMagicNumber = 0x84465e34;
  static constexpr uint32_t recordingVersion2Constant = 2;

  void loadRecording(const std::string &filename);
  void loadOldRecording(const std::string &filename);

  Space space;
  Track track;
  std::string spaceName;
  std::string sonificationMethodName;
  Angle angleX;
  float maxDistance{};
  bool _exitReached{};
  bool oldVersion{};
  bool mapPreviewEnabled{};
  std::vector<bool> movingFocusVector;
  std::vector<bool> mapPreviewVector;
};
