#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <memory>
#include <set>
#include "coordinates.h"

class GraphNode {
  using WeakPtr = std::weak_ptr<GraphNode>;
  using WeakPtrSet = std::set<WeakPtr, std::owner_less<WeakPtr>>;
  using SharedPtr = std::shared_ptr<GraphNode>;

 public:
  GraphNode(Coordinates value);

  void addNeighbor(const SharedPtr& neighbor);
  void markAsExitNode();

  [[nodiscard]] WeakPtrSet getNeighbors() const;
  [[nodiscard]] Coordinates getValue() const;
  [[nodiscard]] bool isExitNode() const;

 private:
  Coordinates value;
  bool exitNode{};
  WeakPtrSet neighbors;
};

//**********************************************************************************************************************

GraphNode::GraphNode(Coordinates value) : value(value) {}

void GraphNode::addNeighbor(const SharedPtr& neighbor) {
  neighbors.insert(neighbor);
}

void GraphNode::markAsExitNode() { exitNode = true; }

typename GraphNode::WeakPtrSet GraphNode::getNeighbors() const {
  return neighbors;
}

Coordinates GraphNode::getValue() const { return value; }

bool GraphNode::isExitNode() const { return exitNode; }

#endif  // GRAPHNODE_H
