#ifndef GRAPH_H
#define GRAPH_H

#include "station_edge.h"
#include <vector>

class Graph {
private:
    std::vector<Station> stations;
    std::vector<Edge> edges;

public:
    void addStation(const std::string& name, double x, double y);
    void addEdge(const std::string& from, const std::string& to, double distance, double averageTime, const std::string& line);
    std::vector<Edge> getNeighbors(const Station& station);
    double getHeuristic(const Station& from, const Station& to);
    Station* getStation(const std::string& name);
    Edge getEdge(Station* from, Station* to);
};

#endif // GRAPH_H