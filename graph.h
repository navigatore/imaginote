#pragma once
#include <map>
#include <memory>
#include <set>
#include <vector>
#include "graphnode.h"

using GraphNodeWeakPtr = std::weak_ptr<GraphNode>;

using GraphNodeWeakPtrToFloatMap =
    std::map<GraphNodeWeakPtr, float, std::owner_less<GraphNodeWeakPtr>>;

using GraphNodeWeakPointersMap = std::map<GraphNodeWeakPtr, GraphNodeWeakPtr,
                                          std::owner_less<GraphNodeWeakPtr>>;

using GraphNodeWeakPointersSet =
    std::set<GraphNodeWeakPtr, std::owner_less<GraphNodeWeakPtr>>;

using GraphNodeSharedPtr = std::shared_ptr<GraphNode>;

using GraphNodeSharedPointers = std::vector<GraphNodeSharedPtr>;

class Graph {
 public:
  explicit Graph() {}

  class ExitNotFound : public std::exception {};
  class NodeNotFound : public std::exception {};
  class NoExitNode : public std::exception {};

  void addExitNode(const Coordinates &value);
  void addNonExitNode(const Coordinates &value);

  [[nodiscard]] std::vector<Coordinates> findShortestPathToAnyExit(
      Coordinates startNodeValue);

  [[nodiscard]] const GraphNodeSharedPointers &getNodes() const;

 private:
  using GraphNodeSharedPointersSet = std::set<GraphNodeSharedPtr>;
  using GraphNodeSharedPointersMap =
      std::map<GraphNodeSharedPtr, GraphNodeSharedPtr>;
  using GraphNodeSharedPtrToFloatMap = std::map<GraphNodeSharedPtr, float>;

  static void addNode(GraphNodeSharedPointers &nodes, const Coordinates &value);

  [[nodiscard]] std::vector<Coordinates> reconstructPath(
      const GraphNodeSharedPointersMap cameFrom,
      GraphNodeSharedPtr current) const;

  [[nodiscard]] std::shared_ptr<GraphNode> &getNodeByValue(
      const Coordinates &value);

  [[nodiscard]] float distanceFromClosestExit(
      const Coordinates &position) const;

  GraphNodeSharedPointers _nodes;
  GraphNodeSharedPointers exitNodes;
};
