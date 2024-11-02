#include "PopulationSystem.hpp"
#include <ResourceLoader.hpp>
#include <MeshInstance.hpp>
#include <Transform.hpp>
#include <Basis.hpp>

namespace Systems {

using namespace godot;

void PopulationSystem::_register_methods() {
    register_method("_ready", &PopulationSystem::_ready);
    register_method("update", &PopulationSystem::update);
    register_method("populate_area", &PopulationSystem::populate_area);
    register_method("populate_multiple_areas", &PopulationSystem::populate_multiple_areas);
}

PopulationSystem::PopulationSystem() {}
PopulationSystem::~PopulationSystem() {}

void PopulationSystem::_init() {
    rng.instance();
    rng->randomize();
}

void PopulationSystem::_ready() {
    tree_prefab = ResourceLoader::get_singleton()->load("res://src/Graphics/Models/Trees/Tree.tscn");
    building_prefab = ResourceLoader::get_singleton()->load("res://src/Graphics/Models/Buildings/Building.tscn");
}

void PopulationSystem::update(float delta_time) {
    // Implement periodic population updates
}

void PopulationSystem::populate_area(const Vector3& position, const String& type) {
    Ref<PackedScene> prefab;
    Vector3 offset_range;
    
    if (type == "tree") {
        prefab = tree_prefab;
        offset_range = Vector3(5, 0, 5);
    } else if (type == "building") {
        prefab = building_prefab;
        offset_range = Vector3(10, 0, 10);
    } else {
        return;
    }

    if (prefab.is_valid()) {
        Node* instance = prefab->instance();
        MeshInstance* mesh_instance = Object::cast_to<MeshInstance>(instance);
        
        if (mesh_instance) {
            Vector3 random_offset(
                rng->randf_range(-offset_range.x, offset_range.x),
                0,
                rng->randf_range(-offset_range.z, offset_range.z)
            );
            
            Transform transform;
            transform.set_basis(Basis());
            transform.set_origin(position + random_offset);
            
            mesh_instance->set_transform(transform);
            add_child(mesh_instance);
        }
    }
}

void PopulationSystem::populate_multiple_areas() {
    // Implementation depends on your GameManager structure
    // Example:
    /*
    for (const auto& node : game_manager->get_nodes()) {
        // Populate trees
        for (int i = 0; i < 10; i++) {
            populate_area(node->get_position(), "tree");
        }
        
        // Populate buildings
        for (int i = 0; i < 5; i++) {
            populate_area(node->get_position(), "building");
        }
    }
    */
}

} // namespace Systems 