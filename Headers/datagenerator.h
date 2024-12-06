#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <vector>
#include <random>

class DataGenerator {
private:
    std::mt19937 generator;  // Mersenne Twister RNG
    std::uniform_real_distribution<double> uniform_dist;  // Uniform distribution for generating U
    double shape;  // Shape parameter (alpha)
    double scale;  // Scale parameter (xm)
    double rate;   // Rate of data generation (events per second)

public:
    // Constructor
    DataGenerator(double shape, double scale, double rate, unsigned int seed = std::random_device{}());

    // Generate a single random number following the Pareto distribution
    double generate();

    // Generate data over a specified time interval
    std::vector<double> generateOverTime(double time_in_seconds);
};

#endif // DATAGENERATOR_H
