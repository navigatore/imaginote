#pragma once
#include <chrono>
#include "duration.h"
#include "mapwidget.h"
#include "path.h"
#include "space.h"

class Analysis {
 public:
  explicit Analysis(std::chrono::milliseconds updatePeriod);

  class InvalidFile : public std::exception {};
  class FileNotLoaded : public std::exception {};

  void setMapWidget(MapWidget* mapWidget);
  void loadRecording(const std::string& filename);
  void findBestTrack();
  void calculateMeanDifference();

  [[nodiscard]] Duration getDuration() const;
  [[nodiscard]] float getMeanDifference() const;

 private:
  static constexpr uint32_t recordingMagicNumber = 0x84465e34;
  static constexpr uint32_t recordingVersion2Constant = 2;

  void loadRecordingVersion2(const std::string& filename);
  void loadRecordingVersion1(const std::string& filename);

  Space space;
  Track track;
  MapWidget* mapWidget{};
  float meanDifference{};
  Path shortestPath;
};
