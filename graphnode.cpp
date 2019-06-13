#include "graphnode.h"

GraphNode::GraphNode(Coordinates value) : value(value) {}

void GraphNode::addNeighbor(const SharedPtr& neighbor) {
  neighbors.insert(neighbor);
}

void GraphNode::markAsExitNode() { exitNode = true; }

GraphNode::WeakPtrSet GraphNode::getNeighbors() const { return neighbors; }

Coordinates GraphNode::getValue() const { return value; }

bool GraphNode::isExitNode() const { return exitNode; }
