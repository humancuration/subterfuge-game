#include "Election.hpp"

namespace Systems {

using namespace godot;

void Election::_register_methods() {
    register_method("_init", &Election::_init);
    
    register_property<Election, String>("title", &Election::set_title, &Election::get_title, "");
    register_property<Election, int>("frequency_in_years", &Election::set_frequency, &Election::get_frequency, 1);
    register_property<Election, float>("next_election_time", &Election::set_next_election_time, &Election::get_next_election_time, 0.0f);
}

Election::Election() : 
    frequency_in_years(1),
    next_election_time(0.0f),
    type(ElectionType::MAYOR) {}

Election::~Election() {}

void Election::_init() {
    // Initialize with default values if needed
}

} // namespace Systems 