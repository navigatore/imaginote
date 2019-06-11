#ifndef GRAPH_H
#define GRAPH_H

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

  void addExitNode(const Coordinates &value);
  void addNonExitNode(const Coordinates &value);

  [[nodiscard]] std::vector<Coordinates> aStar(
      GraphNodeWeakPtr startNode,
      float (*heuristic)(const Coordinates &, const Coordinates &),
      float (*distance)(const Coordinates &, const Coordinates &));

  [[nodiscard]] const GraphNodeSharedPointers &getNodes() const;

 private:
  static void addNode(GraphNodeSharedPointers &nodes, const Coordinates &value);

  [[nodiscard]] std::vector<Coordinates> reconstructPath(
      const GraphNodeWeakPointersMap cameFrom, GraphNodeWeakPtr current) const;

  GraphNodeSharedPointers _nodes;
  GraphNodeSharedPointers exitNodes;
};
//**********************************************************************************************************************

void Graph::addNode(GraphNodeSharedPointers &nodes, const Coordinates &value) {
  nodes.push_back(std::make_shared<GraphNode>(value));
}

void Graph::addExitNode(const Coordinates &value) {
  addNode(_nodes, value);
  _nodes[_nodes.size() - 1]->markAsExitNode();
  addNode(exitNodes, value);
}

void Graph::addNonExitNode(const Coordinates &value) { addNode(_nodes, value); }

std::vector<Coordinates> Graph::aStar(GraphNodeWeakPtr startNode,
                                      float (*heuristic)(const Coordinates &,
                                                         const Coordinates &),
                                      float (*distance)(const Coordinates &,
                                                        const Coordinates &)) {
  GraphNodeWeakPointersSet closedSet;
  GraphNodeWeakPointersSet openSet{startNode};
  GraphNodeWeakPointersMap cameFrom;
  GraphNodeWeakPtrToFloatMap gScore;
  GraphNodeWeakPtrToFloatMap fScore;

  gScore[startNode] = 0.0F;
  fScore[startNode];

  while (!openSet.empty()) {
    GraphNodeWeakPtr current;
    for (const auto &node : openSet) {
      if (current.expired() || (fScore.find(node) != fScore.end() &&
                                fScore[node] < fScore[current])) {
        current = node;
      }
    }
    if (current.lock()->isExitNode()) {
      return reconstructPath(cameFrom, current);
    }

    openSet.erase(current);
    closedSet.insert(current);

    for (const auto &n : current.lock()->getNeighbors()) {
      if (closedSet.find(n) != closedSet.end()) {
        continue;
      }

      auto tentativeGScore =
          gScore[current] +
          distance(current.lock()->getValue(), n.lock()->getValue());

      if (openSet.find(n) == openSet.end()) {
        openSet.insert(n);
      } else if (tentativeGScore >= gScore[n]) {
        continue;
      }

      cameFrom[n] = current;
      gScore[n] = tentativeGScore;
      fScore[n] =
          gScore[n] + heuristic(n.lock()->getValue(), exitNodes[0]->getValue());
    }
  }

  throw 1;
}

const GraphNodeSharedPointers &Graph::getNodes() const { return _nodes; }

std::vector<Coordinates> Graph::reconstructPath(
    const GraphNodeWeakPointersMap cameFrom, GraphNodeWeakPtr current) const {
  std::vector<Coordinates> totalPath;
  while (cameFrom.find(current) != cameFrom.end()) {
    totalPath.push_back(current.lock()->getValue());
    current = cameFrom.at(current);
  }
  totalPath.push_back(current.lock()->getValue());
  return totalPath;
}

#endif  // GRAPH_H
