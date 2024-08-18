#ifndef HIDDEN_MARKOV_MODEL_H
#define HIDDEN_MARKOV_MODEL_H

#include <vector>
#include <string>

class HiddenMarkovModel {
private:
    std::vector<std::vector<double>> transitionMatrix;
    std::vector<std::vector<double>> emissionMatrix;
    std::vector<double> initialProbabilities;  

public:
    HiddenMarkovModel(int numStates, int numObservations);
    void train(const std::vector<std::vector<int>>& observations);
    double getWaitingTimeProbability(size_t station, double time);
    double getDelayProbability(const std::string& line, double time);
};

#endif // HIDDEN_MARKOV_MODEL_H