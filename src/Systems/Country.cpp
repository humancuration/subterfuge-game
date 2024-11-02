#include "Country.h"

#include <Godot.hpp>
#include "Election.h"
#include "Candidate.h"
#include "Globals.h" // Assuming there's a Globals class managing game state

namespace Systems {

using namespace godot;

void Country::_register_methods() {
    register_method("initialize_elections", &Country::initialize_elections);
    register_method("handle_elections", &Country::handle_elections);
}

Country::Country() {}
Country::~Country() {}

void Country::_init() {
    // Initialization if needed
}

void Country::initialize_regions() {
    // Example: Create regions
    Region *northern_plains = Region::_new();
    northern_plains->set_name("Northern Plains");
    regions.append(northern_plains);

    Region *eastern_hills = Region::_new();
    eastern_hills->set_name("Eastern Hills");
    regions.append(eastern_hills);

    // Add more regions as needed
}

void Country::initialize_elections() {
    // Initialize national elections
    Election *presidential_election = Election::_new();
    presidential_election->set_title("Presidential Election");
    presidential_election->set_type(ElectionType::PRESIDENT);
    presidential_election->set_frequency_in_years(4);
    presidential_election->set_next_election_time(3600.0f * 4); // 4 years

    national_elections.append(presidential_election);

    // Initialize regional elections
    for(int i = 0; i < regions.size(); i++) {
        Region *region = Object::cast_to<Region>(regions[i]);
        if(region) {
            region->initialize_elections();
        }
    }
}

void Country::handle_elections() {
    for(int i = 0; i < national_elections.size(); i++) {
        Election *election = Object::cast_to<Election>(national_elections[i]);
        if(election) {
            if(election->get_next_election_time() <= 0.0f) {
                conduct_election(i);
                election->set_next_election_time(election->get_frequency_in_years() * 3600.0f); // Set next election time
            } else {
                election->set_next_election_time(election->get_next_election_time() - 3600.0f); // Advance time
            }
        }
    }

    // Handle regional elections
    for(int i = 0; i < regions.size(); i++) {
        Region *region = Object::cast_to<Region>(regions[i]);
        if(region) {
            region->handle_elections();
        }
    }
}

void Country::conduct_election(int election_index) {
    Election *election = Object::cast_to<Election>(national_elections[election_index]);
    if(election) {
        // Implement national election logic
        godot::Array candidates = load_candidates(election->get_type());

        election->set_candidates(candidates);

        godot::Dictionary results;
        for(int i = 0; i < candidates.size(); i++) {
            Candidate *candidate = Object::cast_to<Candidate>(candidates[i]);
            if(candidate) {
                results[candidate->get_name()] = 0;
            }
        }

        for(int i = 0; i < regions.size(); i++) {
            Region *region = Object::cast_to<Region>(regions[i]);
            if(region) {
                // Aggregate votes from each region
                godot::Array region_candidates = region->get_election(election->get_type())->get_candidates();
                for(int j = 0; j < region_candidates.size(); j++) {
                    Candidate *candidate = Object::cast_to<Candidate>(region_candidates[j]);
                    if(candidate && results.has(candidate->get_name())) {
                        results[candidate->get_name()] = results[candidate->get_name()] + region->get_vote_count(candidate);
                    }
                }
            }
        }

        // Determine winner
        godot::String winner_name = "";
        float max_votes = -1.0f;
        godot::Array candidate_names = results.keys();
        for(int i = 0; i < candidate_names.size(); i++) {
            godot::String name = candidate_names[i];
            float votes = results[name];
            if(votes > max_votes) {
                max_votes = votes;
                winner_name = name;
            }
        }

        godot::print("National Election Winner in " + name + ": " + winner_name);

        // Apply winner's policies or effects
        // Assuming a method to retrieve candidate by name
        Candidate *winner = nullptr;
        for(int i = 0; i < candidates.size(); i++) {
            Candidate *candidate = Object::cast_to<Candidate>(candidates[i]);
            if(candidate && candidate->get_name() == winner_name) {
                winner = candidate;
                break;
            }
        }

        if(winner) {
            winner->apply_policies(this);
        }
    }
}

godot::Array Country::load_candidates(const godot::String &type) {
    // Load candidates from JSON or generate dynamically
    // Placeholder implementation
    godot::Array candidates;
    Candidate *candidate_x = Candidate::_new();
    candidate_x->set_name("President X");
    candidate_x->add_platform("EconomicProsperity", 65.0f);
    candidate_x->add_platform("EnvironmentalHealth", 60.0f);
    candidates.append(candidate_x);

    Candidate *candidate_y = Candidate::_new();
    candidate_y->set_name("President Y");
    candidate_y->add_platform("EconomicProsperity", 45.0f);
    candidate_y->add_platform("EnvironmentalHealth", 85.0f);
    candidates.append(candidate_y);

    return candidates;
}

} // namespace Systems 