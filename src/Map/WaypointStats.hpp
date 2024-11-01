#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "../Core/SIMDHelper.hpp"

class WaypointStats {
private:
    // SIMD-aligned stat arrays for batch processing
    alignas(32) std::vector<float> stat_values;
    alignas(32) std::vector<float> stat_changes;
    std::unordered_map<std::string, size_t> stat_indices;
    
    static constexpr size_t SIMD_BATCH_SIZE = 8;

public:
    WaypointStats() {
        initialize_stats();
    }

    void update_stats_simd(float delta_time) {
        const size_t stat_count = stat_values.size();
        
        for (size_t i = 0; i < stat_count; i += SIMD_BATCH_SIZE) {
            __m256 values = _mm256_load_ps(&stat_values[i]);
            __m256 changes = _mm256_load_ps(&stat_changes[i]);
            __m256 delta = _mm256_set1_ps(delta_time);
            
            // Apply changes
            values = _mm256_add_ps(values, 
                _mm256_mul_ps(changes, delta));
            
            // Clamp values between 0 and 100
            values = _mm256_min_ps(_mm256_set1_ps(100.0f),
                _mm256_max_ps(_mm256_setzero_ps(), values));
            
            _mm256_store_ps(&stat_values[i], values);
        }
    }

    float get_stat(const std::string& name) const {
        auto it = stat_indices.find(name);
        return it != stat_indices.end() ? stat_values[it->second] : 0.0f;
    }

    void set_stat(const std::string& name, float value) {
        auto it = stat_indices.find(name);
        if (it != stat_indices.end()) {
            stat_values[it->second] = value;
        }
    }

private:
    void initialize_stats() {
        // Pre-allocate aligned vectors
        stat_values.resize(32, 50.0f); // Default value of 50
        stat_changes.resize(32, 0.0f);
        
        // Initialize stat indices for fast lookup
        const std::vector<std::string> stat_names = {
            "Morale", "ResourceAvailability", "PoliticalStability",
            "EnvironmentalHealth", "EconomicProsperity", "PopulationDensity",
            "Radicalization", "HealthRisk", "CrimeRate", "TechnologicalLevel",
            "CulturalDevelopment", "InfluenceSpread", "MedicalResources",
            "Education", "Infrastructure", "Tourism"
        };

        for (size_t i = 0; i < stat_names.size(); ++i) {
            stat_indices[stat_names[i]] = i;
        }
    }
}; 