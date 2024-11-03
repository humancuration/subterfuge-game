#include "population_characteristics.hpp"
#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>

namespace game_systems {

void PopulationCharacteristics::_bind_methods() {
    ClassDB::bind_method(D_METHOD("update", "delta", "stats", "terrain"), &PopulationCharacteristics::update);
    ClassDB::bind_method(D_METHOD("serialize_population"), &PopulationCharacteristics::serialize_population);
    ClassDB::bind_method(D_METHOD("deserialize_population", "data"), &PopulationCharacteristics::deserialize_population);

    ClassDB::bind_method(D_METHOD("set_size", "size"), &PopulationCharacteristics::set_size);
    ClassDB::bind_method(D_METHOD("get_size"), &PopulationCharacteristics::get_size);
    ClassDB::bind_method(D_METHOD("set_growth_rate", "rate"), &PopulationCharacteristics::set_growth_rate);
    ClassDB::bind_method(D_METHOD("get_growth_rate"), &PopulationCharacteristics::get_growth_rate);
    ClassDB::bind_method(D_METHOD("set_education_level", "level"), &PopulationCharacteristics::set_education_level);
    ClassDB::bind_method(D_METHOD("get_education_level"), &PopulationCharacteristics::get_education_level);
    ClassDB::bind_method(D_METHOD("set_health_level", "level"), &PopulationCharacteristics::set_health_level);
    ClassDB::bind_method(D_METHOD("get_health_level"), &PopulationCharacteristics::get_health_level);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "growth_rate"), "set_growth_rate", "get_growth_rate");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "education_level"), "set_education_level", "get_education_level");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "health_level"), "set_health_level", "get_health_level");
}

PopulationCharacteristics::PopulationCharacteristics()
    : size(1000)
    , growth_rate(1.0f)
    , education_level(50.0f)
    , health_level(50.0f) {
}

void PopulationCharacteristics::update(double delta, WaypointStats* stats, TerrainFeature* terrain) {
    if (!stats || !terrain) return;

    adjust_growth_rate(stats, terrain);
    adjust_health(stats, terrain);
    adjust_education(stats, terrain);

    // Update population size based on growth rate
    float new_size = static_cast<float>(size) + size * (growth_rate / 100.0f) * delta;
    size = static_cast<int>(std::clamp(new_size, 0.0f, 1000000.0f));
}

void PopulationCharacteristics::adjust_growth_rate(WaypointStats* stats, TerrainFeature* terrain) {
    float resource_factor = stats->get_resource_availability() / 100.0f;
    float health_risk_factor = stats->get_health_risk() / 100.0f;
    
    growth_rate = 1.0f + resource_factor - health_risk_factor;
    growth_rate = std::clamp(growth_rate, -5.0f, 5.0f);
}

void PopulationCharacteristics::adjust_health(WaypointStats* stats, TerrainFeature* terrain) {
    float health_risk = stats->get_health_risk();
    health_level += (100.0f - health_risk) * 0.01f;
    health_level = std::clamp(health_level, 0.0f, 100.0f);
}

void PopulationCharacteristics::adjust_education(WaypointStats* stats, TerrainFeature* terrain) {
    float education_factor = stats->get_education() * 0.02f;
    float cultural_factor = stats->get_cultural_development() * 0.01f;
    
    education_level += education_factor + cultural_factor;
    education_level = std::clamp(education_level, 0.0f, 100.0f);
}

Dictionary PopulationCharacteristics::serialize_population() const {
    Dictionary data;
    data["size"] = size;
    data["growth_rate"] = growth_rate;
    data["education_level"] = education_level;
    data["health_level"] = health_level;
    return data;
}

void PopulationCharacteristics::deserialize_population(const Dictionary& data) {
    if (data.has("size")) size = static_cast<int>(data["size"]);
    if (data.has("growth_rate")) growth_rate = static_cast<float>(data["growth_rate"]);
    if (data.has("education_level")) education_level = static_cast<float>(data["education_level"]);
    if (data.has("health_level")) health_level = static_cast<float>(data["health_level"]);
}

// Getters/Setters
void PopulationCharacteristics::set_size(int p_size) {
    size = std::clamp(p_size, 0, 1000000);
}

int PopulationCharacteristics::get_size() const {
    return size;
}

void PopulationCharacteristics::set_growth_rate(float p_rate) {
    growth_rate = std::clamp(p_rate, -5.0f, 5.0f);
}

float PopulationCharacteristics::get_growth_rate() const {
    return growth_rate;
}

void PopulationCharacteristics::set_education_level(float p_level) {
    education_level = std::clamp(p_level, 0.0f, 100.0f);
}

float PopulationCharacteristics::get_education_level() const {
    return education_level;
}

void PopulationCharacteristics::set_health_level(float p_level) {
    health_level = std::clamp(p_level, 0.0f, 100.0f);
}

float PopulationCharacteristics::get_health_level() const {
    return health_level;
}

}  // namespace game_systems 