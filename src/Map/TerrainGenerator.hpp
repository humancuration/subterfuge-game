#pragma once
#include <vector>
#include <random>
#include "../Core/SIMDHelper.hpp"
#include "TerrainSystem.hpp"

class TerrainGenerator {
private:
    struct NoiseLayer {
        alignas(16) float frequency;
        alignas(16) float amplitude;
        alignas(16) float persistence;
        uint32_t seed;
    };

    static constexpr size_t MAX_NOISE_LAYERS = 8;
    alignas(32) std::vector<NoiseLayer> noise_layers;
    alignas(32) std::vector<float> noise_values;
    
    std::unique_ptr<JobSystem> job_system;

public:
    void generate_terrain_parallel(TerrainSystem& terrain, size_t width, size_t height) {
        const size_t total_size = width * height;
        noise_values.resize(total_size);
        
        // Process terrain in parallel chunks
        for (size_t i = 0; i < total_size; i += 1024) {
            job_system->schedule_job(
                [this, i, total_size]() {
                    generate_noise_batch_simd(i, std::min(size_t(1024), total_size - i));
                },
                JobSystem::Priority::HIGH
            );
        }
        
        job_system->process_jobs();
        apply_noise_to_terrain(terrain);
    }

private:
    void generate_noise_batch_simd(size_t start_idx, size_t count) {
        alignas(32) float x_coords[8];
        alignas(32) float y_coords[8];
        alignas(32) float results[8];
        
        for (size_t i = 0; i < count; i += 8) {
            // Load coordinates
            for (size_t j = 0; j < 8 && i + j < count; ++j) {
                size_t idx = start_idx + i + j;
                x_coords[j] = static_cast<float>(idx % width);
                y_coords[j] = static_cast<float>(idx / width);
            }
            
            __m256 noise = _mm256_setzero_ps();
            
            // Accumulate noise layers using SIMD
            for (const auto& layer : noise_layers) {
                __m256 freq = _mm256_set1_ps(layer.frequency);
                __m256 amp = _mm256_set1_ps(layer.amplitude);
                
                __m256 x = _mm256_load_ps(x_coords);
                __m256 y = _mm256_load_ps(y_coords);
                
                x = _mm256_mul_ps(x, freq);
                y = _mm256_mul_ps(y, freq);
                
                __m256 layer_noise = generate_perlin_noise_simd(x, y, layer.seed);
                layer_noise = _mm256_mul_ps(layer_noise, amp);
                noise = _mm256_add_ps(noise, layer_noise);
            }
            
            _mm256_store_ps(&noise_values[start_idx + i], noise);
        }
    }

    __m256 generate_perlin_noise_simd(__m256 x, __m256 y, uint32_t seed);
}; 