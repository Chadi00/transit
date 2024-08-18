#include "pathfinder.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    PathFinder pathFinder;

    std::string start, end;
    std::cout << "Enter start station: ";
    std::getline(std::cin, start);
    std::cout << "Enter end station: ";
    std::getline(std::cin, end);

    try {
        std::vector<Station*> path = pathFinder.findOptimalPath(start, end);
        
        if (path.empty()) {
            std::cout << "No path found between " << start << " and " << end << std::endl;
        } else {
            double estimatedTime = pathFinder.estimateTravelTime(path);

            std::cout << "Optimal path:" << std::endl;
            for (size_t i = 0; i < path.size(); ++i) {
                std::cout << path[i]->name;
                if (i < path.size() - 1) std::cout << " -> ";
            }
            std::cout << "\nEstimated travel time: " << estimatedTime << " minutes" << std::endl;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Please make sure you entered valid station names." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
    }

    return 0;
}
