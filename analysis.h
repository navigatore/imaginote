#pragma once
#include <chrono>
#include "duration.h"
#include "mapwidget.h"
#include "path.h"
#include "recording.h"
#include "space.h"

class Analysis {
 public:
  Analysis() = default;

  class FileNotLoaded : public std::exception {};
  class InvalidFile : public std::exception {};

  void setMapWidget(MapWidget* mapWidget);
  void loadRecording(const std::string& filename);
  void findBestTrack();
  void calculateMeanDifference();

  [[nodiscard]] bool getIsExtended() const noexcept;
  [[nodiscard]] bool getExitReached() const noexcept;
  [[nodiscard]] std::string getSonificationMethodName() const noexcept;
  [[nodiscard]] Duration getDuration() const;
  [[nodiscard]] float getMeanDifference() const;
  [[nodiscard]] Angle getVisualAngle() const noexcept;
  [[nodiscard]] float getDistanceLimit() const noexcept;
  [[nodiscard]] float getTrackingRadiusEnabledPercentageTime() const noexcept;

 private:
  bool isExtended{};
  Space space;
  Track track;
  std::unique_ptr<Recording> recording;
  MapWidget* mapWidget{};
  float meanDifference{};
  Path shortestPath;
};
