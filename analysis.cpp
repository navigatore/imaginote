#include "analysis.h"
#include <algorithm>
#include <boost/archive/text_iarchive.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include "graph.h"

Analysis::Analysis(std::chrono::milliseconds updatePeriod)
    : track(updatePeriod) {}

void Analysis::setMapWidget(MapWidget *mapWidget) {
  this->mapWidget = mapWidget;
}

void Analysis::loadRecording(const std::string &filename) {
  try {
    loadRecordingVersion2(filename);
  } catch (...) {
    std::cerr
        << "This is not a recording file v. 2, trying to load as v. 1...\n";
    try {
      loadRecordingVersion1(filename);
    } catch (...) {
      mapWidget->hide();
      throw InvalidFile();
    }
  }
  shortestPath = Path();
  meanDifference = 0;

  space.findCorners();
  findBestTrack();
  calculateMeanDifference();

  mapWidget->loadMap(space.getFields());
  mapWidget->setTrack(track);
  mapWidget->show();
}

void Analysis::findBestTrack() {
  auto innerCorners = space.getInnerCorners();
  auto exitCorners = space.getExitCorners();

  mapWidget->setCorners(innerCorners);
  mapWidget->setExitCorners(exitCorners);

  auto initialPosition = Coordinates(track.getPositions()[0]);

  Graph closestPathPossiblePoints;
  closestPathPossiblePoints.addNonExitNode(initialPosition);
  for (const auto &corner : innerCorners) {
    closestPathPossiblePoints.addNonExitNode(Coordinates(corner));
  }
  for (const auto &corner : exitCorners) {
    closestPathPossiblePoints.addExitNode(Coordinates(corner));
  }

  for (auto &node : closestPathPossiblePoints.getNodes()) {
    for (const auto &otherNode : closestPathPossiblePoints.getNodes()) {
      if (node != otherNode && !space.hasFieldBetweenPoints(
                                   node->getValue(), otherNode->getValue())) {
        node->addNeighbor(otherNode);
      }
    }
  }
  auto shortestPathNodes = closestPathPossiblePoints.findShortestPathToAnyExit(
      Coordinates(initialPosition));

  for (const auto &node : shortestPathNodes) {
    shortestPath.addNode(node);
  }
  mapWidget->setShortestPath(shortestPath.getNodes());
}

void Analysis::calculateMeanDifference() {
  float meanAccumulator{};
  for (const auto &position : track.getPositions()) {
    meanAccumulator += shortestPath.distance(position);
  }
  meanDifference = meanAccumulator / track.getPositions().size();
}

Duration Analysis::getDuration() const { return track.getDuration(); }

float Analysis::getMeanDifference() const { return meanDifference; }

void Analysis::loadRecordingVersion2(const std::string &) {
  throw std::logic_error("Load recording file v. 2 not implemented yet");
}

void Analysis::loadRecordingVersion1(const std::string &filename) {
  std::ifstream f;
  f.open(filename.c_str());
  boost::archive::text_iarchive ia(f);
  ia >> space >> track;
}
