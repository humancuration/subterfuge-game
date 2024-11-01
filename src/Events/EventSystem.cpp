#include "EventSystem.hpp"
#include <immintrin.h>

void EventSystem::process_event_batch_simd(size_t start_idx, size_t count) {
    alignas(32) float probabilities[8];
    alignas(32) float thresholds[8];
    
    for (size_t i = 0; i < count; i += 8) {
        // Load 8 probabilities at once
        __m256 prob_vec = _mm256_load_ps(&event_probabilities[start_idx + i]);
        __m256 thresh_vec = _mm256_set1_ps(0.5f); // Example threshold
        
        // Compare probabilities with thresholds
        __m256 result = _mm256_cmp_ps(prob_vec, thresh_vec, _CMP_GT_OQ);
        
        _mm256_store_ps(probabilities, prob_vec);
        _mm256_store_ps(thresholds, result);
        
        // Process results
        for (int j = 0; j < 8 && i + j < count; ++j) {
            if (thresholds[j] != 0) {
                // Event should trigger
                auto* instance = event_pool.allocate();
                instance->data = &event_database[std::to_string(start_idx + i + j)];
                instance->is_active = true;
                active_events.push_back(instance);
            }
        }
    }
}

void EventSystem::update_events_parallel(float delta) {
    const size_t event_count = event_probabilities.size();
    
    for (size_t i = 0; i < event_count; i += BATCH_SIZE) {
        job_system->schedule_job(
            [this, i]() {
                process_event_batch_simd(i, std::min(BATCH_SIZE, event_probabilities.size() - i));
            },
            JobSystem::Priority::MEDIUM
        );
    }
    
    job_system->process_jobs();
} 