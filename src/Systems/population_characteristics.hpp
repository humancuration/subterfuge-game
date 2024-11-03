#ifndef POPULATION_CHARACTERISTICS_HPP
#define POPULATION_CHARACTERISTICS_HPP

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "waypoint_stats.hpp"
#include "terrain_feature.hpp"

namespace game_systems {

class PopulationCharacteristics : public godot::Node {
    GDCLASS(PopulationCharacteristics, Node)

private:
    int size;
    float growth_rate;
    float education_level;
    float health_level;

    void adjust_growth_rate(WaypointStats* stats, TerrainFeature* terrain);
    void adjust_health(WaypointStats* stats, TerrainFeature* terrain);
    void adjust_education(WaypointStats* stats, TerrainFeature* terrain);

protected:
    static void _bind_methods();

public:
    PopulationCharacteristics();
    ~PopulationCharacteristics() = default;

    void update(double delta, WaypointStats* stats, TerrainFeature* terrain);

    // Serialization
    Dictionary serialize_population() const;
    void deserialize_population(const Dictionary& data);

    // Getters/Setters
    void set_size(int p_size);
    int get_size() const;
    void set_growth_rate(float p_rate);
    float get_growth_rate() const;
    void set_education_level(float p_level);
    float get_education_level() const;
    void set_health_level(float p_level);
    float get_health_level() const;
};

}  // namespace game_systems

#endif // POPULATION_CHARACTERISTICS_HPP 