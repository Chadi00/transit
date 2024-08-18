#ifndef STATION_EDGE_H
#define STATION_EDGE_H

#include <string>

class Station {
public:
    std::string name;
    double x, y;  // coordinates for heuristic calculation
};

class Edge {
public:
    Station* from;
    Station* to;
    double distance;
    double averageTime;
    std::string line;
};

#endif // STATION_EDGE_H