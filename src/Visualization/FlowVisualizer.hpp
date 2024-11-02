#pragma once
#include "../Map/ResourceSharingNetwork.hpp"
#include "../Map/KnowledgeSharingSystem.hpp"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/meshinstance3d.hpp>
#include <godot_cpp/classes/cpuparticles3d.hpp>

class FlowVisualizer : public godot::Node3D {
    GDCLASS(FlowVisualizer, Node3D)

public:
    struct FlowParticle {
        godot::Vector3 position;
        godot::Vector3 velocity;
        float lifetime{1.0f};
        float energy{1.0f};
        std::string type;  // "resource", "knowledge", "cultural"
    };

    struct FlowPath {
        godot::Vector3 start;
        godot::Vector3 end;
        float intensity{1.0f};
        std::string flow_type;
        bool is_bidirectional{false};
        float pulse_rate{1.0f};
    };

private:
    // Visual elements
    std::vector<godot::CPUParticles3D*> particle_systems;
    std::vector<godot::MeshInstance3D*> flow_paths;
    
    // Flow data
    std::vector<FlowParticle> active_particles;
    std::vector<FlowPath> active_paths;

    // Visual settings
    struct FlowTheme {
        struct ColorScheme {
            godot::Color resource_flow{0.4f, 0.7f, 1.0f};    // Blue
            godot::Color knowledge_flow{1.0f, 0.4f, 0.7f};   // Pink
            godot::Color cultural_flow{0.4f, 1.0f, 0.7f};    // Green
            float base_opacity{0.7f};
        } colors;

        struct ParticleSettings {
            float min_speed{1.0f};
            float max_speed{3.0f};
            float min_size{0.1f};
            float max_size{0.3f};
            int particles_per_flow{50};
        } particles;

        struct PathSettings {
            float min_width{0.1f};
            float max_width{0.5f};
            float curve_factor{0.3f};
        } paths;
    } theme;

public:
    void visualize_resource_flows(const ResourceSharingNetwork& network) {
        clear_flows();
        
        // Create flow paths for resource transfers
        for (const auto& transfer : network.get_active_transfers()) {
            create_flow_path(
                transfer.source_pos,
                transfer.target_pos,
                "resource",
                transfer.amount / network.get_max_transfer()
            );
        }
        
        // Spawn particles along paths
        update_particle_systems();
    }

    void visualize_knowledge_transfer(const KnowledgeSharingSystem& system) {
        // Similar to resource flows but with different visuals
        for (const auto& transfer : system.get_active_transfers()) {
            create_flow_path(
                transfer.teacher_pos,
                transfer.student_pos,
                "knowledge",
                transfer.understanding_rate
            );
        }
    }

private:
    void create_flow_path(const godot::Vector3& start, 
                         const godot::Vector3& end,
                         const std::string& type,
                         float intensity) {
        // Create curved path mesh
        auto* path_mesh = create_curved_path_mesh(start, end, intensity);
        
        // Set material based on type
        auto material = godot::Ref<godot::ShaderMaterial>(
            create_flow_material(type, intensity)
        );
        path_mesh->set_material_override(material);
        
        // Add to active paths
        active_paths.push_back({
            start, end, intensity, type,
            type == "knowledge", // Knowledge flows are bidirectional
            type == "cultural" ? 0.5f : 1.0f // Cultural flows pulse slower
        });
    }

    void update_particle_systems() {
        for (const auto& path : active_paths) {
            auto* particles = create_particle_system(path);
            
            // Configure particle behavior
            particles->set_amount(
                theme.particles.particles_per_flow * path.intensity
            );
            particles->set_lifetime(2.0f / path.intensity);
            
            // Set emission shape to follow path
            set_emission_curve(particles, path);
            
            particle_systems.push_back(particles);
        }
    }

    godot::ShaderMaterial* create_flow_material(const std::string& type, 
                                              float intensity) {
        auto* material = memnew(godot::ShaderMaterial);
        
        // Load and configure shader
        // TODO: Implement shader with:
        // - Flow animation
        // - Glow effect
        // - Pulse based on intensity
        
        return material;
    }

    void _process(float delta) {
        // Update particle movements
        for (auto* particles : particle_systems) {
            update_particle_flow(particles, delta);
        }
        
        // Update path effects
        for (size_t i = 0; i < active_paths.size(); ++i) {
            update_path_effects(flow_paths[i], active_paths[i], delta);
        }
    }

    void update_particle_flow(godot::CPUParticles3D* particles, float delta) {
        // Update particle positions along path
        // Apply forces (e.g., turbulence, attraction)
        // Handle particle lifetime and regeneration
    }

    void update_path_effects(godot::MeshInstance3D* path_mesh,
                           const FlowPath& path_data,
                           float delta) {
        // Update path animation
        // Pulse effect for intensity
        // Flow direction indication
    }

    void clear_flows() {
        // Clean up existing visualizations
        for (auto* particles : particle_systems) {
            particles->queue_free();
        }
        particle_systems.clear();
        
        for (auto* path : flow_paths) {
            path->queue_free();
        }
        flow_paths.clear();
        
        active_particles.clear();
        active_paths.clear();
    }

protected:
    static void _bind_methods() {
        // Bind methods for GDScript interaction
    }
}; 