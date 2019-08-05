#include "analysis.h"
#include <algorithm>
#include <boost/archive/text_iarchive.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include "graph.h"

void Analysis::setMapWidget(MapWidget *mapWidget) {
  this->mapWidget = mapWidget;
}

void Analysis::loadRecording(const std::string &filename) {
  try {
    recording = std::make_unique<Recording>(filename);
    isExtended = true;
  } catch (...) {
    std::cerr
        << "This is not a recording file v. 2, trying to load as v. 1...\n";
    try {
      recording = std::make_unique<Recording>(filename, true);
      isExtended = false;
    } catch (...) {
      throw InvalidFile();
    }
  }
  shortestPath = Path();
  meanDifference = 0;

  space = recording->getSpace();
  space.findCorners();
  track = recording->getTrack();
  findBestTrack();
  calculateMeanDifference();

  mapWidget->loadMap(space.getFields());
  mapWidget->setTrack(recording->getTrack());
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

bool Analysis::getIsExtended() const noexcept { return isExtended; }

bool Analysis::getExitReached() const noexcept {
  return recording->getExitReached();
}

std::string Analysis::getSonificationMethodName() const noexcept {
  return recording->getSonificationMethodName();
}

Duration Analysis::getDuration() const { return track.getDuration(); }

float Analysis::getMeanDifference() const { return meanDifference; }

Angle Analysis::getVisualAngle() const noexcept {
  return recording->getVisualAngle();
}

float Analysis::getDistanceLimit() const noexcept {
  return recording->getDistanceLimit();
}
