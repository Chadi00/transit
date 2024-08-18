#include "../include/pathfinder.h"
#include <stdexcept>
#include <iostream>

PathFinder::PathFinder() : astar(metroGraph), hmm(3, 5) {
    // Initialize metro graph
    initializeMetroGraph();
}

std::vector<Station*> PathFinder::findOptimalPath(const std::string& start, const std::string& end) {
    Station* startStation = metroGraph.getStation(start);
    Station* endStation = metroGraph.getStation(end);

    if (!startStation || !endStation) {
        throw std::runtime_error("Invalid station name");
    }

    return astar.findPath(startStation, endStation);
}

double PathFinder::estimateTravelTime(const std::vector<Station*>& path) {
    double totalTime = 0;
    
    if (path.size() < 2) {
        return totalTime;  // Return 0 if path is too short
    }

    for (size_t i = 0; i < path.size() - 1; ++i) {
        try {
            Edge edge = metroGraph.getEdge(path[i], path[i+1]);
            
            // Check if the edge is valid
            if (edge.from == nullptr || edge.to == nullptr) {
                throw std::runtime_error("Invalid edge between stations");
            }

            double baseTime = edge.averageTime;
            double waitingTime = hmm.getWaitingTimeProbability(i, baseTime);
            double delayProbability = hmm.getDelayProbability(edge.line, baseTime);

            totalTime += baseTime + waitingTime + (delayProbability * DELAY_FACTOR);
        }
        catch (const std::exception& e) {
            std::cerr << "Error calculating travel time between stations "
                      << path[i]->name << " and " << path[i+1]->name
                      << ": " << e.what() << std::endl;
            totalTime += 30;  // Add 30 minutes as a penalty for unknown connections
        }
    }
    return totalTime;
}

// Helper function to get station names
std::string PathFinder::getStationName(const std::string& line, int index) {
    std::vector<std::string> greenLine = {"Angrignon", "Monk", "Jolicoeur", "Verdun", "De l'Église", "LaSalle", "Charlevoix", "Lionel-Groulx", "Atwater", "Guy-Concordia", "Peel", "McGill", "Place-des-Arts", "Saint-Laurent", "Berri-UQAM", "Beaudry", "Papineau", "Frontenac", "Préfontaine", "Joliette", "Pie-IX", "Viau", "Assomption", "Cadillac", "Langelier", "Radisson", "Honoré-Beaugrand"};
    std::vector<std::string> orangeLine = {"Montmorency", "De la Concorde", "Cartier", "Henri-Bourassa", "Sauvé", "Crémazie", "Jarry", "Jean-Talon", "Beaubien", "Rosemont", "Laurier", "Mont-Royal", "Sherbrooke", "Berri-UQAM", "Champ-de-Mars", "Place-d'Armes", "Square-Victoria–OACI", "Bonaventure", "Lucien-L'Allier", "Georges-Vanier", "Lionel-Groulx", "Place-Saint-Henri", "Vendôme", "Villa-Maria", "Snowdon", "Côte-Sainte-Catherine", "Plamondon", "Namur", "De la Savane", "Du Collège", "Côte-Vertu"};
    std::vector<std::string> blueLine = {"Snowdon", "Côte-des-Neiges", "Université-de-Montréal", "Édouard-Montpetit", "Outremont", "Acadie", "Parc", "De Castelnau", "Jean-Talon", "Fabre", "D'Iberville", "Saint-Michel"};

    if (line == "Green" && index <= greenLine.size()) {
        return greenLine[index - 1];
    } else if (line == "Orange" && index <= orangeLine.size()) {
        return orangeLine[index - 1];
    } else if (line == "Blue" && index <= blueLine.size()) {
        return blueLine[index - 1];
    }
    return "";
}

void PathFinder::initializeMetroGraph() {
    // Green Line (Line 1)
    metroGraph.addStation("Angrignon", 1, 1);
    metroGraph.addStation("Monk", 2, 1);
    metroGraph.addStation("Jolicoeur", 3, 1);
    metroGraph.addStation("Verdun", 4, 2);
    metroGraph.addStation("De l'Église", 5, 2);
    metroGraph.addStation("LaSalle", 6, 2);
    metroGraph.addStation("Charlevoix", 7, 2);
    metroGraph.addStation("Lionel-Groulx", 8, 3);
    metroGraph.addStation("Atwater", 9, 3);
    metroGraph.addStation("Guy-Concordia", 10, 4);
    metroGraph.addStation("Peel", 11, 4);
    metroGraph.addStation("McGill", 12, 4);
    metroGraph.addStation("Place-des-Arts", 13, 4);
    metroGraph.addStation("Saint-Laurent", 14, 4);
    metroGraph.addStation("Berri-UQAM", 15, 4);
    metroGraph.addStation("Beaudry", 16, 5);
    metroGraph.addStation("Papineau", 17, 5);
    metroGraph.addStation("Frontenac", 18, 5);
    metroGraph.addStation("Préfontaine", 19, 5);
    metroGraph.addStation("Joliette", 20, 5);
    metroGraph.addStation("Pie-IX", 21, 5);
    metroGraph.addStation("Viau", 22, 5);
    metroGraph.addStation("Assomption", 23, 6);
    metroGraph.addStation("Cadillac", 24, 6);
    metroGraph.addStation("Langelier", 25, 6);
    metroGraph.addStation("Radisson", 26, 6);
    metroGraph.addStation("Honoré-Beaugrand", 27, 7);

    // Orange Line (Line 2)
    metroGraph.addStation("Montmorency", 1, 10);
    metroGraph.addStation("De la Concorde", 2, 10);
    metroGraph.addStation("Cartier", 3, 10);
    metroGraph.addStation("Henri-Bourassa", 4, 9);
    metroGraph.addStation("Sauvé", 5, 9);
    metroGraph.addStation("Crémazie", 6, 8);
    metroGraph.addStation("Jarry", 7, 8);
    metroGraph.addStation("Jean-Talon", 8, 7);
    metroGraph.addStation("Beaubien", 9, 7);
    metroGraph.addStation("Rosemont", 10, 7);
    metroGraph.addStation("Laurier", 11, 7);
    metroGraph.addStation("Mont-Royal", 12, 7);
    metroGraph.addStation("Sherbrooke", 13, 7);
    // Berri-UQAM already added
    metroGraph.addStation("Champ-de-Mars", 15, 6);
    metroGraph.addStation("Place-d'Armes", 16, 6);
    metroGraph.addStation("Square-Victoria–OACI", 17, 6);
    metroGraph.addStation("Bonaventure", 18, 5);
    metroGraph.addStation("Lucien-L'Allier", 19, 5);
    metroGraph.addStation("Georges-Vanier", 20, 4);
    // Lionel-Groulx already added
    metroGraph.addStation("Place-Saint-Henri", 22, 4);
    metroGraph.addStation("Vendôme", 23, 3);
    metroGraph.addStation("Villa-Maria", 24, 3);
    metroGraph.addStation("Snowdon", 25, 2);
    metroGraph.addStation("Côte-Sainte-Catherine", 26, 2);
    metroGraph.addStation("Plamondon", 27, 2);
    metroGraph.addStation("Namur", 28, 1);
    metroGraph.addStation("De la Savane", 29, 1);
    metroGraph.addStation("Du Collège", 30, 1);
    metroGraph.addStation("Côte-Vertu", 31, 1);

    // Yellow Line (Line 4)
    metroGraph.addStation("Longueuil–Université-de-Sherbrooke", 18, 6);
    metroGraph.addStation("Jean-Drapeau", 19, 5);
    // Berri-UQAM already added

    // Blue Line (Line 5)
    // Snowdon already added
    metroGraph.addStation("Côte-des-Neiges", 26, 4);
    metroGraph.addStation("Université-de-Montréal", 27, 4);
    metroGraph.addStation("Édouard-Montpetit", 28, 4);
    metroGraph.addStation("Outremont", 29, 4);
    metroGraph.addStation("Acadie", 30, 4);
    metroGraph.addStation("Parc", 31, 5);
    metroGraph.addStation("De Castelnau", 32, 5);
    // Jean-Talon already added
    metroGraph.addStation("Fabre", 34, 5);
    metroGraph.addStation("D'Iberville", 35, 6);
    metroGraph.addStation("Saint-Michel", 36, 6);

    // Add edges for Green Line (Line 1)
    for (int i = 1; i <= 26; i++) {
        metroGraph.addEdge(getStationName("Green", i), getStationName("Green", i+1), 1.0, 2.0, "Green");
    }

    // Add edges for Orange Line (Line 2)
    for (int i = 1; i <= 30; i++) {
        metroGraph.addEdge(getStationName("Orange", i), getStationName("Orange", i+1), 1.0, 2.0, "Orange");
    }

    // Add edges for Yellow Line (Line 4)
    metroGraph.addEdge("Longueuil–Université-de-Sherbrooke", "Jean-Drapeau", 1.0, 2.0, "Yellow");
    metroGraph.addEdge("Jean-Drapeau", "Berri-UQAM", 1.0, 2.0, "Yellow");

    // Add edges for Blue Line (Line 5)
    for (int i = 1; i <= 11; i++) {
        metroGraph.addEdge(getStationName("Blue", i), getStationName("Blue", i+1), 1.0, 2.0, "Blue");
    }

    // Add connection edges
    metroGraph.addEdge("Berri-UQAM", "Berri-UQAM", 0.0, 5.0, "Transfer");
    metroGraph.addEdge("Lionel-Groulx", "Lionel-Groulx", 0.0, 5.0, "Transfer");
    metroGraph.addEdge("Jean-Talon", "Jean-Talon", 0.0, 5.0, "Transfer");
    metroGraph.addEdge("Snowdon", "Snowdon", 0.0, 5.0, "Transfer");
}

