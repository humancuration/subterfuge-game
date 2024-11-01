#include "PathfindingSystem.hpp"
#include <immintrin.h>

void PathfindingSystem::calculate_costs_simd(
    PathNode* nodes,
    size_t count,
    const godot::Vector2& end) {
    
    alignas(32) float positions_x[8];
    alignas(32) float positions_y[8];
    alignas(32) float end_x[8];
    alignas(32) float end_y[8];
    
    for (size_t i = 0; i < count; i += 8) {
        // Load positions
        for (size_t j = 0; j < 8 && i + j < count; ++j) {
            positions_x[j] = nodes[i + j].position.x;
            positions_y[j] = nodes[i + j].position.y;
            end_x[j] = end.x;
            end_y[j] = end.y;
        }
        
        __m256 pos_x = _mm256_load_ps(positions_x);
        __m256 pos_y = _mm256_load_ps(positions_y);
        __m256 target_x = _mm256_load_ps(end_x);
        __m256 target_y = _mm256_load_ps(end_y);
        
        // Calculate distances using SIMD
        __m256 dx = _mm256_sub_ps(pos_x, target_x);
        __m256 dy = _mm256_sub_ps(pos_y, target_y);
        __m256 distances = _mm256_sqrt_ps(
            _mm256_add_ps(
                _mm256_mul_ps(dx, dx),
                _mm256_mul_ps(dy, dy)
            )
        );
        
        // Store results
        alignas(32) float results[8];
        _mm256_store_ps(results, distances);
        
        for (size_t j = 0; j < 8 && i + j < count; ++j) {
            nodes[i + j].h_cost = results[j];
        }
    }
} 