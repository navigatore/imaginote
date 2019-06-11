#include "analysis.h"
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
    std::ifstream f;
    f.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
    f.open(filename.c_str());
    space.loadFromFile(f);
    track.load(f);
    f.close();

    mapWidget->loadMap(space.getFields());
    mapWidget->setTrack(track);
    mapWidget->show();
  } catch (std::ifstream::failure &) {
    mapWidget->hide();
    throw InvalidFile();
  }
}

void Analysis::findBestTrack() {
  if (!space.isLoaded()) {
    throw FileNotLoaded();
  }

  auto innerCorners = space.getInnerCorners();
  auto exitCorners = space.getExitCorners();

  mapWidget->setCorners(innerCorners);
  mapWidget->setExitCorners(exitCorners);

  Graph closestPathPossiblePoints;
  closestPathPossiblePoints.addNonExitNode(
      Coordinates(track.getPositions()[0]));
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
  auto shortestPathNodes = closestPathPossiblePoints.aStar(
      closestPathPossiblePoints.getNodes()[0], Coordinates::distance2d,
      Coordinates::distance2d);

  std::vector<Coordinates2d> shortestPath2d;
  for (const auto &node : shortestPathNodes) {
    shortestPath2d.push_back(node);
  }
  mapWidget->setShortestPath(shortestPath2d);
}
