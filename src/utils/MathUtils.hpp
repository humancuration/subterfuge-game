#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>

class MathUtils {
public:
    static int hamming_distance(const std::string& s1, const std::string& s2) {
        if (s1.length() != s2.length())
            return -1; // Undefined for unequal lengths

        return std::inner_product(
            s1.begin(), s1.end(), s2.begin(), 0,
            std::plus<>(),
            std::not_equal_to<>()
        );
    }

    static float calculate_genetic_distance(
        const std::vector<std::string>& markers1,
        const std::vector<std::string>& markers2) {
        
        if (markers1.empty() || markers2.empty())
            return std::numeric_limits<float>::infinity();

        std::vector<int> distances;
        distances.reserve(markers1.size() * markers2.size());

        for (const auto& m1 : markers1) {
            for (const auto& m2 : markers2) {
                int distance = hamming_distance(m1, m2);
                if (distance != -1)
                    distances.push_back(distance);
            }
        }

        if (distances.empty())
            return std::numeric_limits<float>::infinity();

        return std::accumulate(distances.begin(), distances.end(), 0.0f) / 
               distances.size();
    }
}; 