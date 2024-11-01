#pragma once
#include "TerrainSystem.hpp"
#include "../Core/JobSystem.hpp"
#include <queue>

class TerrainModifier {
private:
    struct ModificationRequest {
        alignas(16) godot::Vector2 position;
        alignas(16) float radius;
        alignas(16) float strength;
        uint8_t type;
    };

    std::queue<ModificationRequest> modification_queue;
    std::unique_ptr<JobSystem> job_system;
    
    // SIMD-aligned buffers for batch processing
    alignas(32) std::vector<float> modification_strengths;
    alignas(32) std::vector<float> terrain_heights;

public:
    void queue_modification(const godot::Vector2& pos, float radius, float strength, uint8_t type) {
        modification_queue.push({pos, radius, strength, type});
    }

    void process_modifications(TerrainSystem& terrain) {
        const size_t batch_size = 64;
        std::vector<ModificationRequest> batch;
        
        while (!modification_queue.empty()) {
            batch.clear();
            
            // Collect a batch of modifications
            for (size_t i = 0; i < batch_size && !modification_queue.empty(); ++i) {
                batch.push_back(modification_queue.front());
                modification_queue.pop();
            }
            
            // Process batch in parallel
            process_modification_batch(terrain, batch);
        }
    }

private:
    void process_modification_batch(TerrainSystem& terrain, 
                                  const std::vector<ModificationRequest>& batch) {
        const size_t total_points = batch.size() * 64; // Points per modification
        modification_strengths.resize(total_points);
        terrain_heights.resize(total_points);
        
        // Calculate modification strengths using SIMD
        for (size_t i = 0; i < total_points; i += 8) {
            calculate_modification_strengths_simd(
                &modification_strengths[i],
                &terrain_heights[i],
                batch[i / 64],
                i % 64
            );
        }
        
        // Apply modifications
        apply_modifications_simd(terrain, modification_strengths, terrain_heights);
    }

    void calculate_modification_strengths_simd(float* strengths, float* heights,
                                             const ModificationRequest& req,
                                             size_t offset);
                                             
    void apply_modifications_simd(TerrainSystem& terrain,
                                const std::vector<float>& strengths,
                                const std::vector<float>& heights);
}; 