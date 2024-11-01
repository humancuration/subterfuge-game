#pragma once
#include <vector>
#include <cstddef>

template<typename T>
class CacheOptimizer {
private:
    static constexpr size_t CACHE_LINE_SIZE = 64;
    static constexpr size_t PREFETCH_DISTANCE = 8;

public:
    // Reorganize data for better cache utilization
    static void optimize_data_layout(std::vector<T>& data) {
        if (data.size() <= 1) return;
        
        std::vector<T> optimized;
        optimized.reserve(data.size());
        
        // Reorganize data to ensure frequently accessed elements
        // are cache-line aligned
        for (size_t i = 0; i < data.size(); i += CACHE_LINE_SIZE / sizeof(T)) {
            size_t end = std::min(i + CACHE_LINE_SIZE / sizeof(T), data.size());
            for (size_t j = i; j < end; ++j) {
                optimized.push_back(std::move(data[j]));
            }
        }
        
        data = std::move(optimized);
    }

    // Prefetch data for upcoming operations
    static void prefetch_data(const T* data, size_t count) {
        for (size_t i = 0; i < count; i += PREFETCH_DISTANCE) {
            __builtin_prefetch(&data[i], 0, 3);  // Read, high temporal locality
        }
    }
}; 