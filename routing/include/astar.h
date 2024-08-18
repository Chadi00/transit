#ifndef ASTAR_H
#define ASTAR_H

#include "graph.h"
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>

class AStar {
private:
    struct Node {
        Station* station;
        std::shared_ptr<Node> parent;
        double g;  // Cost from start to this node
        double h;  // Heuristic (estimated cost from this node to goal)
        double f() const { return g + h; }
    };

    struct CompareNode {
        bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
            return a->f() > b->f();
        }
    };

    using PriorityQueue = std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNode>;

    Graph& graph;

    std::vector<Station*> reconstructPath(const std::shared_ptr<Node>& goal);

public:
    AStar(Graph& g);
    std::vector<Station*> findPath(Station* start, Station* goal);
};

#endif // ASTAR_H