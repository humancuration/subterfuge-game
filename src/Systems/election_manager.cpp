#include "election_manager.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void ElectionManager::_bind_methods() {
    ClassDB::bind_method(D_METHOD("update", "delta"), &ElectionManager::update);
    ClassDB::bind_method(D_METHOD("initialize_with_countries", "countries"), &ElectionManager::initialize_with_countries);
}

ElectionManager::ElectionManager() : election_timer(3600.0) {}

ElectionManager::~ElectionManager() {
    countries.clear();
}

void ElectionManager::_ready() {
    UtilityFunctions::print("ElectionManager initialized.");
}

void ElectionManager::update(double delta) {
    election_timer -= delta;
    if (election_timer <= 0.0) {
        trigger_elections();
        set_next_election_cycle();
    }
}

void ElectionManager::trigger_elections() {
    for (Country* country : countries) {
        if (country) {
            country->handle_elections();
        }
    }
}

void ElectionManager::set_next_election_cycle() {
    election_timer = 3600.0; // 1 hour in game time
}

void ElectionManager::initialize_with_countries(TypedArray<Country> p_countries) {
    countries.clear();
    for (int i = 0; i < p_countries.size(); i++) {
        Country* country = Object::cast_to<Country>(p_countries[i]);
        if (country) {
            countries.push_back(country);
            country->initialize_elections();
        }
    }
} 