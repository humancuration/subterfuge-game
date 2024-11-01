#pragma once
#include <vector>
#include <queue>
#include "../Core/JobSystem.hpp"

class PathfindingSystem {
private:
    struct PathNode {
        alignas(16) godot::Vector2 position;
        float g_cost{0.0f};
        float h_cost{0.0f};
        PathNode* parent{nullptr};
    };

    ObjectPool<PathNode> node_pool;
    std::unique_ptr<JobSystem> job_system;
    
    // SIMD-aligned data for batch processing
    alignas(32) std::vector<float> costs_x;
    alignas(32) std::vector<float> costs_y;

public:
    std::vector<godot::Vector2> find_path_parallel(
        const godot::Vector2& start,
        const godot::Vector2& end,
        const std::vector<City*>& cities) {
            
        const size_t batch_size = 64;
        std::vector<std::future<std::vector<PathNode*>>> path_futures;
        
        // Split pathfinding into parallel tasks
        for (size_t i = 0; i < cities.size(); i += batch_size) {
            path_futures.push_back(
                job_system->enqueue([this, i, &cities, start, end]() {
                    return process_path_batch(
                        &cities[i],
                        std::min(batch_size, cities.size() - i),
                        start, end
                    );
                })
            );
        }
        
        // Combine results
        return combine_paths(path_futures);
    }

private:
    std::vector<PathNode*> process_path_batch(
        City* const* cities,
        size_t count,
        const godot::Vector2& start,
        const godot::Vector2& end);
        
    void calculate_costs_simd(
        PathNode* nodes,
        size_t count,
        const godot::Vector2& end);
}; 