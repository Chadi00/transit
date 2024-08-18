#include "../include/graph.h"
#include <iostream>
#include <algorithm>
#include <cmath>

void Graph::addStation(const std::string& name, double x, double y) {
    stations.push_back({name, x, y});
}

void Graph::addEdge(const std::string& from, const std::string& to, double distance, double averageTime, const std::string& line) {
    Station* fromStation = getStation(from);
    Station* toStation = getStation(to);
    if (fromStation && toStation) {
        edges.push_back({fromStation, toStation, distance, averageTime, line});
        edges.push_back({toStation, fromStation, distance, averageTime, line}); // Add reverse edge
    }
}

std::vector<Edge> Graph::getNeighbors(const Station& station) {
    std::vector<Edge> neighbors;
    for (const auto& edge : edges) {
        if (edge.from->name == station.name) {
            neighbors.push_back(edge);
        } else if (edge.to->name == station.name) {
            // Add reverse edge
            neighbors.push_back({edge.to, edge.from, edge.distance, edge.averageTime, edge.line});
        }
    }

    // Debug print
    std::cout << "Station: " << station.name << " has " << neighbors.size() << " neighbors.\n";
    for (const auto& neighbor : neighbors) {
        std::cout << "  Neighbor: " << neighbor.to->name << " via line " << neighbor.line
                  << " (distance: " << neighbor.distance << ", time: " << neighbor.averageTime << ")\n";
    }

    return neighbors;
}

double Graph::getHeuristic(const Station& from, const Station& to) {
    // Simple Euclidean distance heuristic
    double dx = from.x - to.x;
    double dy = from.y - to.y;
    return std::sqrt(dx*dx + dy*dy);
}

Station* Graph::getStation(const std::string& name) {
    auto it = std::find_if(stations.begin(), stations.end(),
                           [&name](const Station& s) { return s.name == name; });
    return it != stations.end() ? &(*it) : nullptr;
}

Edge Graph::getEdge(Station* from, Station* to) {
    auto it = std::find_if(edges.begin(), edges.end(),
                           [from, to](const Edge& e) { return e.from == from && e.to == to; });
    return it != edges.end() ? *it : Edge();
}