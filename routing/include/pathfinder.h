#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "graph.h"
#include "astar.h"
#include "hidden_markov_model.h"
#include <vector>
#include <string>

class PathFinder {
private:
    Graph metroGraph;
    AStar astar;
    HiddenMarkovModel hmm;
    const double DELAY_FACTOR = 5.0; // minutes

    void initializeMetroGraph();

public:
    PathFinder();
    std::vector<Station*> findOptimalPath(const std::string& start, const std::string& end);
    double estimateTravelTime(const std::vector<Station*>& path);
    std::string getStationName(const std::string& line, int index);
};

#endif // PATHFINDER_H