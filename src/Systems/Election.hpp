#ifndef ELECTION_HPP
#define ELECTION_HPP

#include <Godot.hpp>
#include <Reference.hpp>
#include <String.hpp>
#include <vector>
#include "Candidate.hpp"

namespace Systems {

enum class ElectionType {
    MAYOR,
    PRESIDENT
};

class Election : public godot::Reference {
    GODOT_CLASS(Election, godot::Reference)

private:
    godot::String title;
    ElectionType type;
    int frequency_in_years;
    float next_election_time;
    std::vector<Candidate*> candidates;

public:
    static void _register_methods();
    
    Election();
    ~Election();

    void _init();

    // Getters and setters
    void set_title(const godot::String& p_title) { title = p_title; }
    godot::String get_title() const { return title; }

    void set_type(ElectionType p_type) { type = p_type; }
    ElectionType get_type() const { return type; }

    void set_frequency(int years) { frequency_in_years = years; }
    int get_frequency() const { return frequency_in_years; }

    void set_next_election_time(float time) { next_election_time = time; }
    float get_next_election_time() const { return next_election_time; }

    void set_candidates(const std::vector<Candidate*>& p_candidates) { candidates = p_candidates; }
    std::vector<Candidate*> get_candidates() const { return candidates; }
};

} // namespace Systems

#endif // ELECTION_HPP 