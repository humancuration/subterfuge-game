#include "event_condition.hpp"
#include <godot_cpp/variant/utility_functions.hpp>
#include <cmath>

void EventCondition::_bind_methods() {
    ClassDB::bind_method(D_METHOD("is_met", "game_manager"), &EventCondition::is_met);
    ClassDB::bind_method(D_METHOD("set_time_phase", "phase"), &EventCondition::set_time_phase);
    ClassDB::bind_method(D_METHOD("get_time_phase"), &EventCondition::get_time_phase);
    ClassDB::bind_method(D_METHOD("set_weather", "weather"), &EventCondition::set_weather);
    ClassDB::bind_method(D_METHOD("get_weather"), &EventCondition::get_weather);
    ClassDB::bind_method(D_METHOD("set_population_density", "density"), &EventCondition::set_population_density);
    ClassDB::bind_method(D_METHOD("get_population_density"), &EventCondition::get_population_density);
    ClassDB::bind_method(D_METHOD("set_custom_conditions", "conditions"), &EventCondition::set_custom_conditions);
    ClassDB::bind_method(D_METHOD("get_custom_conditions"), &EventCondition::get_custom_conditions);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "time_phase"), "set_time_phase", "get_time_phase");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "weather"), "set_weather", "get_weather");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "population_density"), "set_population_density", "get_population_density");
    ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "custom_conditions"), "set_custom_conditions", "get_custom_conditions");
}

EventCondition::EventCondition() {}

EventCondition::~EventCondition() {}

bool EventCondition::is_met(GameManager* game_manager) const {
    return check_time_phase(game_manager) &&
           check_weather(game_manager) &&
           check_population_density(game_manager) &&
           check_custom_conditions(game_manager);
}

// Implementation of other methods...
// (For brevity, I'm showing just a few key methods. The complete implementation would include all the checking methods)

double EventCondition::parse_population_density(const String& density) const {
    String lower_density = density.to_lower();
    if (lower_density == "high") return 70.0;
    if (lower_density == "medium") return 50.0;
    if (lower_density == "low") return 30.0;
    ERR_PRINT("Invalid population density value: " + density);
    return 0.0;
} 