#include "voting_influencer.hpp"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void VotingInfluencer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("add_influence", "source_id", "target_id", 
                                "strength", "duration", "is_positive"),
                       &VotingInfluencer::add_influence);
}