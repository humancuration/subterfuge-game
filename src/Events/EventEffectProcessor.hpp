#pragma once
#include <vector>
#include <functional>
#include "../Core/JobSystem.hpp"

class EventEffectProcessor {
private:
    struct EffectBatch {
        alignas(16) std::vector<float> values;
        alignas(16) std::vector<float> multipliers;
        std::vector<std::function<void(float)>> callbacks;
    };

    std::unique_ptr<JobSystem> job_system;
    static constexpr size_t BATCH_SIZE = 64;

public:
    void process_effects_parallel(const std::vector<std::function<void()>>& effects) {
        const size_t effect_count = effects.size();
        
        for (size_t i = 0; i < effect_count; i += BATCH_SIZE) {
            job_system->schedule_job(
                [this, &effects, i]() {
                    const size_t batch_end = std::min(i + BATCH_SIZE, effects.size());
                    for (size_t j = i; j < batch_end; ++j) {
                        effects[j]();
                    }
                },
                JobSystem::Priority::HIGH
            );
        }
        
        job_system->process_jobs();
    }

    void process_value_effects_simd(EffectBatch& batch) {
        const size_t count = batch.values.size();
        
        for (size_t i = 0; i < count; i += 8) {
            __m256 values = _mm256_load_ps(&batch.values[i]);
            __m256 multipliers = _mm256_load_ps(&batch.multipliers[i]);
            
            __m256 results = _mm256_mul_ps(values, multipliers);
            _mm256_store_ps(&batch.values[i], results);
            
            for (size_t j = 0; j < 8 && i + j < count; ++j) {
                batch.callbacks[i + j](batch.values[i + j]);
            }
        }
    }
}; 