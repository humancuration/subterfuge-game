#include "TerrainModifier.hpp"
#include <immintrin.h>

void TerrainModifier::calculate_modification_strengths_simd(
    float* strengths,
    float* heights,
    const ModificationRequest& req,
    size_t offset) {
    
    alignas(32) float distances[8];
    alignas(32) float radii[8];
    alignas(32) float base_strengths[8];
    
    // Load constant values
    __m256 radius_vec = _mm256_set1_ps(req.radius);
    __m256 strength_vec = _mm256_set1_ps(req.strength);
    __m256 center_x = _mm256_set1_ps(req.position.x);
    __m256 center_y = _mm256_set1_ps(req.position.y);
    
    // Calculate positions for this batch
    alignas(32) float pos_x[8];
    alignas(32) float pos_y[8];
    for (int i = 0; i < 8; ++i) {
        float angle = (offset + i) * (2.0f * M_PI / 64.0f);
        pos_x[i] = req.position.x + std::cos(angle) * req.radius;
        pos_y[i] = req.position.y + std::sin(angle) * req.radius;
    }
    
    // Load positions
    __m256 x_vec = _mm256_load_ps(pos_x);
    __m256 y_vec = _mm256_load_ps(pos_y);
    
    // Calculate distances
    __m256 dx = _mm256_sub_ps(x_vec, center_x);
    __m256 dy = _mm256_sub_ps(y_vec, center_y);
    __m256 dist_sq = _mm256_add_ps(
        _mm256_mul_ps(dx, dx),
        _mm256_mul_ps(dy, dy)
    );
    __m256 dist = _mm256_sqrt_ps(dist_sq);
    
    // Calculate falloff
    __m256 falloff = _mm256_sub_ps(_mm256_set1_ps(1.0f),
        _mm256_div_ps(dist, radius_vec));
    
    // Apply strength
    __m256 final_strength = _mm256_mul_ps(falloff, strength_vec);
    
    // Store results
    _mm256_store_ps(strengths, final_strength);
} 