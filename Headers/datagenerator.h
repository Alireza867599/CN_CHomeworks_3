#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <vector>
#include <random>

class DataGenerator {
private:
    std::mt19937 generator;
    std::uniform_real_distribution<double> uniform_dist;
    double shape;
    double scale;
    double rate;   )

public:
    // Constructor
    DataGenerator(double shape, double scale, double rate, unsigned int seed = std::random_device{}());

    double generate();

    std::vector<double> generateOverTime(double time_in_seconds);
};

#endif // DATAGENERATOR_H
