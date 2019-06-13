#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <chrono>
#include "duration.h"
#include "mapwidget.h"
#include "space.h"

class Analysis {
 public:
  explicit Analysis(std::chrono::milliseconds updatePeriod);

  class InvalidFile : public std::exception {};
  class FileNotLoaded : public std::exception {};

  void setMapWidget(MapWidget* mapWidget);
  void loadRecording(const std::string& filename);
  void findBestTrack();

  [[nodiscard]] Duration getDuration() const;

 private:
  Space space;

  Track track;
  MapWidget* mapWidget{};
};

#endif  // ANALYSIS_H
