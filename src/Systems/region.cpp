#include "region.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace game_systems {

void Region::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_name", "name"), &Region::set_name);
    ClassDB::bind_method(D_METHOD("get_name"), &Region::get_name);
    ClassDB::bind_method(D_METHOD("initialize_elections"), &Region::initialize_elections);
    ClassDB::bind_method(D_METHOD("handle_elections", "delta"), &Region::handle_elections);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
}

Region::Region() : name(""), stats() {
    initialize_voters();
}

Region::Region(const String& p_name) : name(p_name), stats() {
    initialize_voters();
}

void Region::initialize_voters() {
    // Create initial voter population
    const int BASE_VOTERS = 1000;
    voters.reserve(BASE_VOTERS);
    
    for (int i = 0; i < BASE_VOTERS; i++) {
        voters.push_back(std::make_unique<Voter>());
    }
}

void Region::initialize_elections() {
    auto mayoral_election = std::make_unique<Election>();
    mayoral_election->set_title("Mayoral Election");
    mayoral_election->set_type(ElectionType::MAYOR);
    mayoral_election->set_frequency(1.0);
    mayoral_election->set_next_election_time(godot::UtilityFunctions::randf() * 3600.0);
    
    elections.push_back(std::move(mayoral_election));
}

void Region::handle_elections(double delta) {
    for (auto& election : elections) {
        if (election->get_next_election_time() <= 0.0) {
            conduct_election(election.get());
            election->set_next_election_time(election->get_frequency() * 3600.0);
        } else {
            election->set_next_election_time(election->get_next_election_time() - delta);
        }
    }
}

void Region::conduct_election(Election* election) {
    auto candidates = load_candidates(election->get_type());
    
    // Count votes using modern C++ algorithms
    std::vector<std::pair<Candidate*, int>> results;
    results.reserve(candidates.size());

    for (auto& candidate : candidates) {
        int votes = std::count_if(voters.begin(), voters.end(),
            [&](const auto& voter) { return voter->supports(&candidate, this); });
        results.push_back({&candidate, votes});
    }

    // Find winner
    auto winner_it = std::max_element(results.begin(), results.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

    if (winner_it != results.end()) {
        auto winner = winner_it->first;
        godot::UtilityFunctions::print(
            String("Election Winner in {0}: {1}")
                .format(Array::make(name, winner->get_name()))
        );
        winner->apply_policies(this);
    }
}
}  // namespace game_systems 