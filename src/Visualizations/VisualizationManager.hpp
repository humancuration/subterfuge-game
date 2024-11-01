#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/spatial_material.hpp>
#include "../Core/SIMDHelper.hpp"
#include <vector>
#include <unordered_map>

class VisualizationManager : public godot::Node3D {
    GDCLASS(VisualizationManager, Node3D)

private:
    struct VisualNode {
        godot::MeshInstance3D* mesh;
        godot::Vector3 position;
        float importance;
    };

    // SIMD-aligned data structures for batch processing
    alignas(32) std::vector<float> node_positions_x;
    alignas(32) std::vector<float> node_positions_y;
    alignas(32) std::vector<float> node_positions_z;
    
    std::unordered_map<godot::String, VisualNode> visual_nodes;
    std::vector<godot::MeshInstance3D*> edge_instances;
    
    // Cached materials for better performance
    godot::Ref<godot::SphereMesh> node_mesh_template;
    godot::Ref<godot::SpatialMaterial> node_material;
    
    // Batch processing size for SIMD operations
    static constexpr size_t BATCH_SIZE = 8;

protected:
    static void _bind_methods();

public:
    VisualizationManager();
    ~VisualizationManager();

    void create_visual_node(const godot::String& name, const godot::Vector3& position);
    void update_node_positions();
    void batch_process_nodes();
}; 