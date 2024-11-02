#pragma once
#include "../Map/SpeciesNetworkSystem.hpp"
#include <godot_cpp/classes/node3d.hpp>

class NetworkVisualizer : public godot::Node3D {
    GDCLASS(NetworkVisualizer, Node3D)

public:
    struct VisualNode {
        godot::Vector3 position;
        float size{1.0f};
        godot::Color color;
        std::string type;  // "species", "culture", "resource"
        float energy{1.0f};
        bool is_highlighted{false};
    };

    struct VisualConnection {
        std::string from_id;
        std::string to_id;
        float strength{1.0f};
        godot::Color color;
        std::string type;  // "knowledge", "resource", "cultural"
    };

private:
    std::unordered_map<std::string, VisualNode> nodes;
    std::vector<VisualConnection> connections;
    
    // Visual settings
    struct Theme {
        godot::Color species_color{0.4f, 0.7f, 1.0f};
        godot::Color cultural_color{1.0f, 0.4f, 0.7f};
        godot::Color resource_color{0.4f, 1.0f, 0.7f};
        float min_node_size{0.5f};
        float max_node_size{2.0f};
        float connection_width{0.1f};
    } theme;

public:
    void visualize_species_network(const SpeciesNetworkSystem& network) {
        clear_visualization();
        
        // Create nodes for each species
        for (const auto& [species_id, data] : network.get_species()) {
            add_species_node(species_id, data);
        }
        
        // Create connections for relationships
        for (const auto& connection : network.get_connections()) {
            add_network_connection(connection);
        }
        
        update_force_directed_layout();
    }

private:
    void update_force_directed_layout() {
        // Implement force-directed layout similar to KnowledgeAtlas
        // - Repulsion between nodes
        // - Attraction along connections
        // - Center gravity
    }
}; 