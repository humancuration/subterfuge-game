#include "MaterialSystem.hpp"
#include <immintrin.h>

void MaterialSystem::batch_update_materials_simd() {
    const size_t count = metallic_values.size();
    
    for (size_t i = 0; i < count; i += 8) {
        __m256 metallic = _mm256_load_ps(&metallic_values[i]);
        __m256 roughness = _mm256_load_ps(&roughness_values[i]);
        __m256 ao = _mm256_load_ps(&ao_values[i]);
        
        // Process PBR parameters in parallel
        __m256 processed_metallic = _mm256_mul_ps(metallic, _mm256_set1_ps(0.96f));
        __m256 processed_roughness = _mm256_mul_ps(roughness, _mm256_set1_ps(1.1f));
        
        _mm256_store_ps(&metallic_values[i], processed_metallic);
        _mm256_store_ps(&roughness_values[i], processed_roughness);
    }
} 