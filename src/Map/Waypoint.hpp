#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include <string>
#include <vector>
#include <memory>
#include "WaypointStats.hpp"
#include "TerrainFeature.hpp"
#include "PopulationCharacteristics.hpp"
#include "../Core/SIMDHelper.hpp"

class Waypoint : public godot::Node3D {
    GDCLASS(Waypoint, Node3D)

private:
    int32_t id;
    std::string waypoint_name;
    std::unique_ptr<WaypointStats> stats;
    std::unique_ptr<PopulationCharacteristics> population;
    std::shared_ptr<TerrainFeature> terrain_feature;
    
    // SIMD-aligned data for connected waypoints
    alignas(32) std::vector<float> connected_positions_x;
    alignas(32) std::vector<float> connected_positions_y;
    std::vector<std::shared_ptr<Waypoint>> connected_waypoints;
    
    // Event tracking with efficient lookup
    std::unordered_set<std::string> active_events;
    
    // Resource management with SIMD-aligned data
    struct ResourceData {
        alignas(16) float amount;
        alignas(16) float production_rate;
        alignas(16) float consumption_rate;
    };
    std::unordered_map<std::string, ResourceData> resources;

protected:
    static void _bind_methods();

public:
    Waypoint();
    Waypoint(int32_t id, const std::string& name, const godot::Vector2& position, 
             std::shared_ptr<TerrainFeature> terrain);

    void update(float delta_time);
    void connect_to(std::shared_ptr<Waypoint> other);
    void disconnect_from(std::shared_ptr<Waypoint> other);
    
    // Event handling
    void add_event(const std::string& event);
    bool remove_event(const std::string& event);
    bool has_event(const std::string& event) const;
    
    // Resource management
    void update_resources_simd(float delta_time);
    void add_resource(const std::string& name, float amount);
    
    // Getters/Setters
    int32_t get_id() const { return id; }
    const std::string& get_name() const { return waypoint_name; }
    WaypointStats* get_stats() const { return stats.get(); }
    PopulationCharacteristics* get_population() const { return population.get(); }
    
    // Serialization
    Dictionary serialize() const;
    void deserialize(const Dictionary& data);

private:
    void update_node_logic(float delta_time);
    void check_and_trigger_events();
    void update_resource_production_simd(float delta_time);
    void batch_process_resources();
}; 