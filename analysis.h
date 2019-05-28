#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <chrono>
#include "mapwidget.h"

class Analysis {
 public:
  explicit Analysis(std::chrono::milliseconds updatePeriod);

  class InvalidFile : public std::exception {};

  void setMapWidget(MapWidget* mapWidget);
  void loadRecording(const std::string& filename);

 private:
  std::vector<std::vector<SimpleSpaceObject>> fields;
  Track track;
  MapWidget* mapWidget{};
};

#endif  // ANALYSIS_H
