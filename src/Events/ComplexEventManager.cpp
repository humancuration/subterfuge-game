#include "ComplexEventManager.hpp"
#include <immintrin.h>

void ComplexEventManager::process_events_simd() {
    const size_t event_count = event_importance.size();
    alignas(32) float importance_threshold[8] = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f};
    
    for (size_t i = 0; i < event_count; i += 8) {
        __m256 importance = _mm256_load_ps(&event_importance[i]);
        __m256 threshold = _mm256_load_ps(importance_threshold);
        
        __m256 should_process = _mm256_cmp_ps(importance, threshold, _CMP_GT_OQ);
        
        alignas(32) float results[8];
        _mm256_store_ps(results, should_process);
        
        for (size_t j = 0; j < 8 && i + j < event_count; ++j) {
            if (results[j] != 0) {
                event_system->trigger_event(std::to_string(i + j));
            }
        }
    }
}

void ComplexEventManager::update_event_importance() {
    const size_t event_count = event_importance.size();
    
    alignas(32) float decay_factor[8] = {0.99f, 0.99f, 0.99f, 0.99f, 0.99f, 0.99f, 0.99f, 0.99f};
    alignas(32) float min_importance[8] = {0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f};
    
    for (size_t i = 0; i < event_count; i += 8) {
        __m256 importance = _mm256_load_ps(&event_importance[i]);
        __m256 decay = _mm256_load_ps(decay_factor);
        __m256 min_val = _mm256_load_ps(min_importance);
        
        importance = _mm256_mul_ps(importance, decay);
        importance = _mm256_max_ps(importance, min_val);
        
        _mm256_store_ps(&event_importance[i], importance);
    }
} 