#include "graph.h"

void Graph::addNode(GraphNodeSharedPointers &nodes, const Coordinates &value) {
  nodes.push_back(std::make_shared<GraphNode>(value));
}

void Graph::addExitNode(const Coordinates &value) {
  addNode(_nodes, value);
  _nodes[_nodes.size() - 1]->markAsExitNode();
  addNode(exitNodes, value);
}

void Graph::addNonExitNode(const Coordinates &value) { addNode(_nodes, value); }

std::vector<Coordinates> Graph::findShortestPathToAnyExit(
    Coordinates startNodeValue) {
  // Modified A* algorithm is used
  auto startNode = getNodeByValue(startNodeValue);
  GraphNodeSharedPointersSet closedSet;
  GraphNodeSharedPointersSet openSet{startNode};
  GraphNodeSharedPointersMap cameFrom;
  GraphNodeSharedPtrToFloatMap gScore;
  GraphNodeSharedPtrToFloatMap fScore;

  gScore[startNode] = 0.0F;
  fScore[startNode];

  while (!openSet.empty()) {
    GraphNodeSharedPtr current;
    for (const auto &node : openSet) {
      if (!current || (fScore.find(node) != fScore.end() &&
                       fScore[node] < fScore[current])) {
        current = node;
      }
    }
    if (current->isExitNode()) {
      return reconstructPath(cameFrom, current);
    }

    openSet.erase(current);
    closedSet.insert(current);

    for (const auto &n : current->getNeighbors()) {
      auto node = n.lock();
      if (closedSet.find(node) != closedSet.end()) {
        continue;
      }

      auto tentativeGScore =
          gScore[current] +
          Coordinates::distance2d(current->getValue(), node->getValue());

      if (openSet.find(node) == openSet.end()) {
        openSet.insert(node);
      } else if (tentativeGScore >= gScore[node]) {
        continue;
      }

      cameFrom[node] = current;
      gScore[node] = tentativeGScore;
      fScore[node] = gScore[node] + distanceFromClosestExit(node->getValue());
    }
  }

  throw ExitNotFound();
}

const GraphNodeSharedPointers &Graph::getNodes() const { return _nodes; }

std::vector<Coordinates> Graph::reconstructPath(
    const GraphNodeSharedPointersMap &cameFrom, GraphNodeSharedPtr current) {
  std::vector<Coordinates> totalPath;
  while (cameFrom.find(current) != cameFrom.end()) {
    totalPath.push_back(current->getValue());
    current = cameFrom.at(current);
  }
  totalPath.push_back(current->getValue());
  return totalPath;
}

std::shared_ptr<GraphNode> &Graph::getNodeByValue(const Coordinates &value) {
  for (auto &node : _nodes) {
    if (node->getValue() == value) {
      return node;
    }
  }
  throw NodeNotFound();
}

float Graph::distanceFromClosestExit(const Coordinates &position) const {
  if (exitNodes.empty()) {
    throw NoExitNode();
  }
  auto minNumberSoFar = position.distance2d(exitNodes[0]->getValue());
  for (const auto &exitNode : exitNodes) {
    minNumberSoFar =
        std::min(minNumberSoFar, position.distance2d(exitNode->getValue()));
  }
  return minNumberSoFar;
}
