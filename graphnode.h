#pragma once
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
