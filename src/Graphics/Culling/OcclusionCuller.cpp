#include <immintrin.h>

void OcclusionCuller::process_occlusion_simd(size_t start_idx, size_t count) {
    const __m256 near_plane = _mm256_set1_ps(0.1f);
    const __m256 far_plane = _mm256_set1_ps(1000.0f);
    
    for (size_t i = start_idx; i < start_idx + count; i += 8) {
        __m256 depth = _mm256_load_ps(&depth_values[i]);
        __m256 screen_x = _mm256_load_ps(&screen_coords_x[i]);
        __m256 screen_y = _mm256_load_ps(&screen_coords_y[i]);
        
        // Frustum culling
        __m256 in_frustum = _mm256_and_ps(
            _mm256_cmp_ps(depth, near_plane, _CMP_GT_OQ),
            _mm256_cmp_ps(depth, far_plane, _CMP_LT_OQ)
        );
        
        // Store results
        alignas(32) float results[8];
        _mm256_store_ps(results, in_frustum);
        
        for (int j = 0; j < 8 && i + j < start_idx + count; ++j) {
            occluders[i + j].is_visible = results[j] != 0;
        }
    }
} 