#include "DataGenerator.h"
#include <cmath>  // For std::pow

// Constructor
DataGenerator::DataGenerator(double shape, double scale, double rate, unsigned int seed)
    : generator(seed), uniform_dist(0.0, 1.0), shape(shape), scale(scale), rate(rate) {}

// Generate a single random number following the Pareto distribution
double DataGenerator::generate() {
    double u = uniform_dist(generator);  // Generate U in [0, 1)
    return scale / std::pow(1 - u, 1.0 / shape);  // Pareto formula
}

// Generate data over a specified time interval
std::vector<double> DataGenerator::generateOverTime(double time_in_seconds) {
    size_t num_samples = static_cast<size_t>(rate * time_in_seconds);  // Total number of events
    std::vector<double> data;
    data.reserve(num_samples);

    for (size_t i = 0; i < num_samples; ++i) {
        data.push_back(generate());
    }

    return data;
}
