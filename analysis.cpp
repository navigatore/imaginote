#include "analysis.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include "graph.h"

Analysis::Analysis(std::chrono::milliseconds updatePeriod)
    : track(updatePeriod) {}

void Analysis::setMapWidget(MapWidget *mapWidget) {
  this->mapWidget = mapWidget;
}

void Analysis::loadRecording(const std::string &filename) {
  try {
    shortestPath = Path();
    meanDifference = 0;
    std::ifstream f;
    f.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
    f.open(filename.c_str());
    space.loadFromFile(f);
    track.load(f);
    f.close();
    findBestTrack();
    calculateMeanDifference();

    mapWidget->loadMap(space.getFields());
    mapWidget->setTrack(track);
    mapWidget->show();
  } catch (std::ifstream::failure &) {
    mapWidget->hide();
    throw InvalidFile();
  }
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
