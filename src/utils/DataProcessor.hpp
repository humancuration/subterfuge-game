#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "MathUtils.hpp"

class DataProcessor {
public:
    using DataEntry = std::unordered_map<std::string, std::string>;
    using DistanceMatrix = std::unordered_map<std::string, 
                          std::unordered_map<std::string, float>>;

    DistanceMatrix process_data(const std::vector<DataEntry>& raw_data) {
        // Extract unique species
        std::unordered_set<std::string> species_set;
        for (const auto& entry : raw_data) {
            if (auto it = entry.find("species"); it != entry.end()) {
                species_set.insert(it->second);
            }
        }

        std::vector<std::string> species_list(species_set.begin(), 
                                            species_set.end());
        DistanceMatrix distance_matrix;

        // Calculate genetic distances
        for (const auto& sp1 : species_list) {
            distance_matrix[sp1] = std::unordered_map<std::string, float>();
            
            // Get genetic markers for species 1
            std::vector<std::string> markers1;
            for (const auto& entry : raw_data) {
                if (entry.at("species") == sp1) {
                    markers1.push_back(entry.at("genetic_marker"));
                }
            }

            for (const auto& sp2 : species_list) {
                // Get genetic markers for species 2
                std::vector<std::string> markers2;
                for (const auto& entry : raw_data) {
                    if (entry.at("species") == sp2) {
                        markers2.push_back(entry.at("genetic_marker"));
                    }
                }

                float distance = MathUtils::calculate_genetic_distance(
                    markers1, markers2);
                distance_matrix[sp1][sp2] = distance;
            }
        }

        return distance_matrix;
    }
}; 