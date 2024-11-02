#pragma once
#include <random>

class RandomGenerator {
private:
    static std::mt19937 rng;
    static std::uniform_real_distribution<float> dist;

public:
    static float get_random_float() {
        return dist(rng);
    }

    static int get_random_int(int min, int max) {
        return std::uniform_int_distribution<int>{min, max}(rng);
    }

    static void seed(uint32_t seed) {
        rng.seed(seed);
    }
};

std::mt19937 RandomGenerator::rng{std::random_device{}()};
std::uniform_real_distribution<float> RandomGenerator::dist{0.0f, 1.0f}; 