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
  Space space;

  Track track;
  MapWidget* mapWidget{};
  float meanDifference{};
  Path shortestPath;
};
