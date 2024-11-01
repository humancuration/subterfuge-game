#include "Waypoint.hpp"
#include <godot_cpp/core/class_db.hpp>

void Waypoint::_bind_methods() {
    ClassDB::bind_method(D_METHOD("update", "delta"), &Waypoint::update);
    ClassDB::bind_method(D_METHOD("get_id"), &Waypoint::get_id);
    ClassDB::bind_method(D_METHOD("get_name"), &Waypoint::get_name);
}

void Waypoint::update(float delta_time) {
    // Update stats using SIMD
    stats->update_stats_simd(delta_time);
    
    // Update population
    population->update(delta_time, stats.get(), terrain_feature.get());
    
    // Update resources in batches
    update_resources_simd(delta_time);
    
    // Update node logic and check events
    update_node_logic(delta_time);
    check_and_trigger_events();
}

void Waypoint::update_resources_simd(float delta_time) {
    const size_t resource_count = resources.size();
    if (resource_count == 0) return;
    
    alignas(32) float amounts[8];
    alignas(32) float production[8];
    alignas(32) float consumption[8];
    
    for (size_t i = 0; i < resource_count; i += 8) {
        size_t batch_size = std::min(size_t(8), resource_count - i);
        
        // Load resource data
        size_t idx = 0;
        for (const auto& [name, data] : resources) {
            if (idx >= batch_size) break;
            amounts[idx] = data.amount;
            production[idx] = data.production_rate;
            consumption[idx] = data.consumption_rate;
            ++idx;
        }
        
        // Process resources using SIMD
        __m256 amount_vec = _mm256_load_ps(amounts);
        __m256 prod_vec = _mm256_load_ps(production);
        __m256 cons_vec = _mm256_load_ps(consumption);
        __m256 delta = _mm256_set1_ps(delta_time);
        
        // Update amounts
        amount_vec = _mm256_add_ps(amount_vec,
            _mm256_mul_ps(_mm256_sub_ps(prod_vec, cons_vec), delta));
        
        // Store results
        _mm256_store_ps(amounts, amount_vec);
        
        // Update resource data
        idx = 0;
        for (auto& [name, data] : resources) {
            if (idx >= batch_size) break;
            data.amount = amounts[idx];
            ++idx;
        }
    }
} 