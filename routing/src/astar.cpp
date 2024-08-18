#include "../include/astar.h"
#include <iostream>
#include <algorithm>

AStar::AStar(Graph& g) : graph(g) {}

std::vector<Station*> AStar::findPath(Station* start, Station* goal) {
    PriorityQueue openSet;
    std::unordered_set<Station*> closedSet;
    std::unordered_map<Station*, std::shared_ptr<Node>> allNodes;

    auto startNode = std::make_shared<Node>(Node{start, nullptr, 0, graph.getHeuristic(*start, *goal)});
    openSet.push(startNode);
    allNodes[start] = startNode;

    while (!openSet.empty()) {
        auto current = openSet.top();
        openSet.pop();

        double f_score = current->g + current->h;
        std::cout << "Exploring station: " << current->station->name
                  << " (g=" << current->g << ", h=" << current->h
                  << ", f=" << f_score << ")" << std::endl;

        if (current->station == goal) {
            std::cout << "Goal station " << goal->name << " found." << std::endl;
            return reconstructPath(current);
        }

        closedSet.insert(current->station);

        for (const Edge& edge : graph.getNeighbors(*current->station)) {
            if (closedSet.find(edge.to) != closedSet.end()) continue;

            double tentativeG = current->g + edge.averageTime;

            auto neighborIt = allNodes.find(edge.to);
            std::shared_ptr<Node> neighbor;

            if (neighborIt == allNodes.end()) {
                double h = graph.getHeuristic(*edge.to, *goal);
                neighbor = std::make_shared<Node>(Node{edge.to, current, tentativeG, h});
                allNodes[edge.to] = neighbor;
            } else {
                neighbor = neighborIt->second;
                if (tentativeG >= neighbor->g) continue;
                
                neighbor->g = tentativeG;
                neighbor->parent = current;
            }

            double new_f_score = neighbor->g + neighbor->h;
            std::cout << "  Neighbor: " << neighbor->station->name
                      << " via line " << edge.line
                      << " (g=" << neighbor->g << ", h=" << neighbor->h
                      << ", f=" << new_f_score << ")" << std::endl;

            openSet.push(neighbor);
        }
    }

    std::cout << "No path found from " << start->name << " to " << goal->name << "." << std::endl;
    return {};  // No path found
}


std::vector<Station*> AStar::reconstructPath(const std::shared_ptr<Node>& goal) {
    std::vector<Station*> path;
    for (auto node = goal; node != nullptr; node = node->parent) {
        path.push_back(node->station);
    }
    std::reverse(path.begin(), path.end());
    return path;
}
