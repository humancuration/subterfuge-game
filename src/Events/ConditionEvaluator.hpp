#pragma once
#include <immintrin.h>
#include <vector>
#include <string>
#include <unordered_map>

class ConditionEvaluator {
private:
    struct ConditionData {
        alignas(32) std::vector<float> values;
        alignas(32) std::vector<float> thresholds;
        std::vector<uint8_t> operators; // 0: <, 1: >, 2: ==, 3: !=
    };

    std::unordered_map<std::string, ConditionData> condition_cache;
    static constexpr size_t SIMD_BATCH_SIZE = 8;

public:
    bool evaluate_conditions_simd(const std::vector<std::string>& conditions) {
        alignas(32) float results[8];
        
        for (size_t i = 0; i < conditions.size(); i += SIMD_BATCH_SIZE) {
            size_t batch_size = std::min(SIMD_BATCH_SIZE, conditions.size() - i);
            
            __m256 values = _mm256_load_ps(&condition_cache[conditions[i]].values[0]);
            __m256 thresholds = _mm256_load_ps(&condition_cache[conditions[i]].thresholds[0]);
            
            __m256 comparison;
            switch (condition_cache[conditions[i]].operators[0]) {
                case 0: // <
                    comparison = _mm256_cmp_ps(values, thresholds, _CMP_LT_OQ);
                    break;
                case 1: // >
                    comparison = _mm256_cmp_ps(values, thresholds, _CMP_GT_OQ);
                    break;
                case 2: // ==
                    comparison = _mm256_cmp_ps(values, thresholds, _CMP_EQ_OQ);
                    break;
                case 3: // !=
                    comparison = _mm256_cmp_ps(values, thresholds, _CMP_NEQ_OQ);
                    break;
            }
            
            _mm256_store_ps(results, comparison);
            
            for (size_t j = 0; j < batch_size; ++j) {
                if (results[j] == 0) return false;
            }
        }
        
        return true;
    }
}; 