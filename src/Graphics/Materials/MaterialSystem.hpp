#pragma once
#include <godot_cpp/classes/material.hpp>
#include "../Core/ObjectPool.hpp"
#include <unordered_map>
#include <string_view>

class MaterialSystem {
private:
    struct MaterialInstance {
        alignas(16) godot::Vector4 base_color;
        alignas(16) godot::Vector4 material_params; // metallic, roughness, ao, etc.
        uint32_t texture_indices[4];  // Packed texture references
    };

    // Pool for material instances
    ObjectPool<MaterialInstance> material_pool;
    
    // SIMD-aligned parameter arrays for batch updates
    alignas(32) std::vector<float> metallic_values;
    alignas(32) std::vector<float> roughness_values;
    alignas(32) std::vector<float> ao_values;
    
    // Material variant system
    struct MaterialVariantKey {
        uint32_t base_material_id;
        uint32_t variant_flags;
        
        bool operator==(const MaterialVariantKey& other) const {
            return base_material_id == other.base_material_id && 
                   variant_flags == other.variant_flags;
        }
    };
    
    struct KeyHash {
        size_t operator()(const MaterialVariantKey& key) const {
            return std::hash<uint64_t>{}(
                (static_cast<uint64_t>(key.base_material_id) << 32) | 
                key.variant_flags
            );
        }
    };

    std::unordered_map<MaterialVariantKey, godot::Ref<godot::Material>, KeyHash> variant_cache;

public:
    void batch_update_materials_simd();
    godot::Ref<godot::Material> get_material_variant(uint32_t base_id, uint32_t variant_flags);
    void update_material_parameters_simd(const std::vector<MaterialInstance*>& instances);
}; 