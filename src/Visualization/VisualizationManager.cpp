#include "VisualizationManager.hpp"
#include <godot_cpp/core/class_db.hpp>

void VisualizationManager::_bind_methods() {
    ClassDB::bind_method(D_METHOD("create_visual_node"), &VisualizationManager::create_visual_node);
    ClassDB::bind_method(D_METHOD("update_node_positions"), &VisualizationManager::update_node_positions);
}

VisualizationManager::VisualizationManager() {
    // Pre-allocate vectors with SIMD alignment
    node_positions_x.reserve(1024);
    node_positions_y.reserve(1024);
    node_positions_z.reserve(1024);

    // Create reusable mesh and material
    node_mesh_template = godot::SphereMesh::_new();
    node_material = godot::SpatialMaterial::_new();
}

void VisualizationManager::create_visual_node(const godot::String& name, const godot::Vector3& position) {
    auto mesh_instance = godot::MeshInstance3D::_new();
    mesh_instance->set_mesh(node_mesh_template);
    mesh_instance->set_material_override(node_material);
    
    VisualNode node{mesh_instance, position, 1.0f};
    visual_nodes[name] = node;
    
    // Add to SIMD-aligned arrays
    node_positions_x.push_back(position.x);
    node_positions_y.push_back(position.y);
    node_positions_z.push_back(position.z);
}

void VisualizationManager::batch_process_nodes() {
    const size_t node_count = visual_nodes.size();
    
    // Process positions in SIMD batches
    for (size_t i = 0; i < node_count; i += BATCH_SIZE) {
        const size_t batch_end = std::min(i + BATCH_SIZE, node_count);
        
        // Use SIMD for position updates
        SIMDHelper::update_positions_simd(
            node_positions_x.data() + i,
            node_positions_y.data() + i,
            node_positions_z.data() + i,
            batch_end - i
        );
    }
} 