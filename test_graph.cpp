#include <gtest/gtest.h>
#include "graph.h"

TEST(GraphTest, givenTwoExitsSecondCloserChoosesSecond) {
  Graph graph;
  auto startPosition = Coordinates(1, 0, 1);
  auto firstExitPosition = Coordinates(0, 0, 0);
  auto secondExitPosition = Coordinates(1, 0, 0);
  graph.addNonExitNode(startPosition);
  graph.addExitNode(firstExitPosition);
  graph.addExitNode(secondExitPosition);
  for (auto &node : graph.getNodes()) {
    for (const auto &otherNode : graph.getNodes()) {
      if (node != otherNode) {
        node->addNeighbor(otherNode);
      }
    }
  }
  ASSERT_EQ(graph.findShortestPathToAnyExit(startPosition)[0],
            secondExitPosition);
}
