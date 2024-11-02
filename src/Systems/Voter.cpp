#include "Voter.h"

#include <Godot.hpp>
#include "Region.h"

namespace Systems {

using namespace godot;

void Voter::_register_methods() {
    register_method("supports", &Voter::supports);
}

Voter::Voter() {}
Voter::~Voter() {}

void Voter::_init() {
    // Initialization if needed
}

void Voter::set_loyalty(float value) {
    loyalty = Math::clamp(value, 0.0f, 100.0f);
}

float Voter::get_loyalty() const {
    return loyalty;
}

void Voter::set_independence(float value) {
    independence = Math::clamp(value, 0.0f, 100.0f);
}

float Voter::get_independence() const {
    return independence;
}

bool Voter::supports(Candidate *candidate, Region *region) {
    float support_score = 0.0f;
    godot::Dictionary platform = candidate->get_platform();

    godot::Array keys = platform.keys();
    for(int i = 0; i < keys.size(); i++) {
        godot::String key = keys[i];
        float value = platform[key];
        float region_stat = region->get_stats()->get_stat_value(key);
        if(region_stat >= value) {
            support_score += 10.0f;
        } else {
            support_score -= 5.0f;
        }
    }

    support_score += (loyalty / 100.0f) * 10.0f;
    support_score -= (independence / 100.0f) * 5.0f;

    return support_score > 0.0f;
}

} // namespace Systems 