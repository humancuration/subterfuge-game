#pragma once
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <vector>
#include "../Core/SIMDHelper.hpp"

class MeshBatcher {
private:
    struct BatchedMesh {
        alignas(16) std::vector<godot::Vector3> positions;
        alignas(16) std::vector<godot::Vector3> normals;
        alignas(16) std::vector<godot::Vector2> uvs;
        uint32_t material_id;
        size_t instance_count;
    };

    // SIMD-aligned transformation data
    alignas(32) std::vector<float> transform_matrices;
    alignas(32) std::vector<float> instance_data;
    
    std::vector<BatchedMesh> mesh_batches;
    static constexpr size_t MAX_INSTANCES_PER_BATCH = 1024;

public:
    void add_instance(godot::MeshInstance3D* mesh_instance);
    void optimize_batches();
    void update_transforms_simd();
    void render_batches();

private:
    void merge_compatible_batches();
    void split_oversized_batches();
}; 