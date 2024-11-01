#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "WorldEvent.hpp"
#include "../NPCController.hpp"
#include "../Core/ThreadPool.hpp"
#include "../Spatial/SpatialGrid.hpp"
#include "../Core/JobSystem.hpp"
#include "../Core/SIMDHelper.hpp"
#include "../Core/CacheOptimizer.hpp"

class EmergentBehaviorManager {
private:
    std::unique_ptr<ThreadPool> threadPool;
    std::unique_ptr<SpatialGrid> spatialGrid;
    std::unique_ptr<JobSystem> jobSystem;
    
    // Cache-aligned NPC positions for SIMD processing
    alignas(32) std::vector<float> npc_positions_x;
    alignas(32) std::vector<float> npc_positions_y;
    
    std::vector<std::weak_ptr<NPCController>> npcs;
    std::unordered_map<WorldEvent::EventType, float> eventInfluence;

    static EmergentBehaviorManager* instance;
    
    // Batch size for parallel processing
    static constexpr size_t BATCH_SIZE = 64;

    EmergentBehaviorManager() 
        : threadPool(std::make_unique<ThreadPool>())
        , spatialGrid(std::make_unique<SpatialGrid>())
        , jobSystem(std::make_unique<JobSystem>(*threadPool)) {
        
        // Pre-allocate position vectors with cache alignment
        npc_positions_x.reserve(1024);
        npc_positions_y.reserve(1024);
    }

public:
    static EmergentBehaviorManager& get_instance();

    void register_npc(const std::shared_ptr<NPCController>& npc);
    void process_event(const WorldEvent& event);
    void update(float delta_time);
    
    // New methods for spatial queries
    std::vector<std::shared_ptr<NPCController>> get_npcs_in_radius(float x, float y, float radius) {
        return spatialGrid->get_nearby_npcs(x, y, radius);
    }

private:
    void process_npc_batch(const std::vector<std::weak_ptr<NPCController>>& batch, 
                          const WorldEvent& event);

    void update_npc_positions() {
        npc_positions_x.clear();
        npc_positions_y.clear();
        
        for (const auto& weak_npc : npcs) {
            if (auto npc = weak_npc.lock()) {
                auto pos = npc->get_position();
                npc_positions_x.push_back(pos.x);
                npc_positions_y.push_back(pos.y);
            }
        }
    }

    void process_npcs_parallel(float delta_time) {
        const size_t npc_count = npcs.size();
        const size_t batch_size = BATCH_SIZE;
        
        for (size_t i = 0; i < npc_count; i += batch_size) {
            size_t current_batch_size = std::min(batch_size, npc_count - i);
            
            jobSystem->schedule_job(
                [this, i, current_batch_size, delta_time]() {
                    // Prefetch next batch of data
                    if (i + current_batch_size < npc_count) {
                        CacheOptimizer<std::weak_ptr<NPCController>>::prefetch_data(
                            &npcs[i + current_batch_size],
                            std::min(batch_size, npc_count - (i + current_batch_size))
                        );
                    }
                    
                    process_npc_batch_simd(i, current_batch_size, delta_time);
                },
                JobSystem::Priority::HIGH
            );
        }
        
        jobSystem->process_jobs();
    }

    void process_npc_batch_simd(size_t start_index, size_t batch_size, float delta_time) {
        // Use SIMD for distance calculations and other vectorizable operations
        std::vector<float> distances(batch_size);
        
        SIMDHelper::calculate_distances_simd(
            &npc_positions_x[start_index],
            &npc_positions_y[start_index],
            target_x, target_y,
            distances.data(),
            batch_size
        );
        
        // Process results
        for (size_t i = 0; i < batch_size; ++i) {
            if (auto npc = npcs[start_index + i].lock()) {
                npc->update_with_distance(distances[i], delta_time);
            }
        }
    }
}; 