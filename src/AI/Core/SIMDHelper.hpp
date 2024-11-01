#pragma once
#include <immintrin.h>
#include <array>
#include <vector>

class SIMDHelper {
public:
    // SIMD-optimized distance calculation for multiple NPCs
    static void calculate_distances_simd(
        const float* positions_x,
        const float* positions_y,
        float target_x,
        float target_y,
        float* distances,
        size_t count) {
        
        const __m256 target_x_vec = _mm256_set1_ps(target_x);
        const __m256 target_y_vec = _mm256_set1_ps(target_y);
        
        for (size_t i = 0; i < count; i += 8) {
            __m256 x_vec = _mm256_loadu_ps(&positions_x[i]);
            __m256 y_vec = _mm256_loadu_ps(&positions_y[i]);
            
            __m256 dx = _mm256_sub_ps(x_vec, target_x_vec);
            __m256 dy = _mm256_sub_ps(y_vec, target_y_vec);
            
            __m256 distances_vec = _mm256_sqrt_ps(
                _mm256_add_ps(
                    _mm256_mul_ps(dx, dx),
                    _mm256_mul_ps(dy, dy)
                )
            );
            
            _mm256_storeu_ps(&distances[i], distances_vec);
        }
    }
}; 