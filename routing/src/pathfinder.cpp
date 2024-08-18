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
    int line1 = 1;
    metroGraph.addStation("Angrignon", line1, 1);
    metroGraph.addStation("Monk", line1, 2);
    metroGraph.addStation("Jolicoeur", line1, 3);
    metroGraph.addStation("Verdun", line1, 4);
    metroGraph.addStation("De l'Église", line1, 5);
    metroGraph.addStation("LaSalle", line1, 6);
    metroGraph.addStation("Charlevoix", line1, 7);
    metroGraph.addStation("Lionel-Groulx", line1, 8); // Transfer station
    metroGraph.addStation("Atwater", line1, 9);
    metroGraph.addStation("Guy-Concordia", line1, 10);
    metroGraph.addStation("Peel", line1, 11);
    metroGraph.addStation("McGill", line1, 12);
    metroGraph.addStation("Place-des-Arts", line1, 13);
    metroGraph.addStation("Saint-Laurent", line1, 14);
    metroGraph.addStation("Berri-UQAM", line1, 15); // Transfer station
    metroGraph.addStation("Beaudry", line1, 16);
    metroGraph.addStation("Papineau", line1, 17);
    metroGraph.addStation("Frontenac", line1, 18);
    metroGraph.addStation("Préfontaine", line1, 19);
    metroGraph.addStation("Joliette", line1, 20);
    metroGraph.addStation("Pie-IX", line1, 21);
    metroGraph.addStation("Viau", line1, 22);
    metroGraph.addStation("Assomption", line1, 23);
    metroGraph.addStation("Cadillac", line1, 24);
    metroGraph.addStation("Langelier", line1, 25);
    metroGraph.addStation("Radisson", line1, 26);
    metroGraph.addStation("Honoré-Beaugrand", line1, 27);

    // Orange Line (Line 2)
    int line2 = 2;
    metroGraph.addStation("Montmorency", line2, 1);
    metroGraph.addStation("De la Concorde", line2, 2);
    metroGraph.addStation("Cartier", line2, 3);
    metroGraph.addStation("Henri-Bourassa", line2, 4);
    metroGraph.addStation("Sauvé", line2, 5);
    metroGraph.addStation("Crémazie", line2, 6);
    metroGraph.addStation("Jarry", line2, 7);
    metroGraph.addStation("Jean-Talon", line2, 8); // Transfer station
    metroGraph.addStation("Beaubien", line2, 9);
    metroGraph.addStation("Rosemont", line2, 10);
    metroGraph.addStation("Laurier", line2, 11);
    metroGraph.addStation("Mont-Royal", line2, 12);
    metroGraph.addStation("Sherbrooke", line2, 13);
    metroGraph.addStation("Berri-UQAM", line2, 14); // Transfer station
    metroGraph.addStation("Champ-de-Mars", line2, 15);
    metroGraph.addStation("Place-d'Armes", line2, 16);
    metroGraph.addStation("Square-Victoria–OACI", line2, 17);
    metroGraph.addStation("Bonaventure", line2, 18);
    metroGraph.addStation("Lucien-L'Allier", line2, 19);
    metroGraph.addStation("Georges-Vanier", line2, 20);
    metroGraph.addStation("Lionel-Groulx", line2, 21); // Transfer station
    metroGraph.addStation("Place-Saint-Henri", line2, 22);
    metroGraph.addStation("Vendôme", line2, 23);
    metroGraph.addStation("Villa-Maria", line2, 24);
    metroGraph.addStation("Snowdon", line2, 25); // Transfer station
    metroGraph.addStation("Côte-Sainte-Catherine", line2, 26);
    metroGraph.addStation("Plamondon", line2, 27);
    metroGraph.addStation("Namur", line2, 28);
    metroGraph.addStation("De la Savane", line2, 29);
    metroGraph.addStation("Du Collège", line2, 30);
    metroGraph.addStation("Côte-Vertu", line2, 31);

    // Yellow Line (Line 4)
    int line4 = 4;
    metroGraph.addStation("Longueuil–Université-de-Sherbrooke", line4, 1);
    metroGraph.addStation("Jean-Drapeau", line4, 2);
    metroGraph.addStation("Berri-UQAM", line4, 3); // Transfer station

    // Blue Line (Line 5)
    int line5 = 5;
    metroGraph.addStation("Snowdon", line5, 1); // Transfer station
    metroGraph.addStation("Côte-des-Neiges", line5, 2);
    metroGraph.addStation("Université-de-Montréal", line5, 3);
    metroGraph.addStation("Édouard-Montpetit", line5, 4);
    metroGraph.addStation("Outremont", line5, 5);
    metroGraph.addStation("Acadie", line5, 6);
    metroGraph.addStation("Parc", line5, 7);
    metroGraph.addStation("De Castelnau", line5, 8);
    metroGraph.addStation("Jean-Talon", line5, 9); // Transfer station
    metroGraph.addStation("Fabre", line5, 10);
    metroGraph.addStation("D'Iberville", line5, 11);
    metroGraph.addStation("Saint-Michel", line5, 12);

    // Add edges for Green Line (Line 1)
    for (int i = 1; i < 27; i++) {
        metroGraph.addEdge(getStationName("Green", i), getStationName("Green", i + 1), 1.0, 2.0, "Green");
    }

    // Add edges for Orange Line (Line 2)
    for (int i = 1; i < 31; i++) {
        metroGraph.addEdge(getStationName("Orange", i), getStationName("Orange", i + 1), 1.0, 2.0, "Orange");
    }

    // Add edges for Yellow Line (Line 4)
    metroGraph.addEdge("Longueuil–Université-de-Sherbrooke", "Jean-Drapeau", 1.0, 2.0, "Yellow");
    metroGraph.addEdge("Jean-Drapeau", "Berri-UQAM", 1.0, 2.0, "Yellow");

    // Add edges for Blue Line (Line 5)
    for (int i = 1; i < 12; i++) {
        metroGraph.addEdge(getStationName("Blue", i), getStationName("Blue", i + 1), 1.0, 2.0, "Blue");
    }

    // Add connection edges for transfer stations
    metroGraph.addEdge("Berri-UQAM", "Berri-UQAM", 0.0, 5.0, "Transfer");
    metroGraph.addEdge("Lionel-Groulx", "Lionel-Groulx", 0.0, 5.0, "Transfer");
    metroGraph.addEdge("Jean-Talon", "Jean-Talon", 0.0, 5.0, "Transfer");
    metroGraph.addEdge("Snowdon", "Snowdon", 0.0, 5.0, "Transfer");
}
