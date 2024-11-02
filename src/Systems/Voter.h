#ifndef VOTER_H
#define VOTER_H

#include <Godot.hpp>
#include <Object.hpp>
#include "Candidate.h"
#include "Region.h"

namespace Systems {

class Voter : public godot::Object {
    GODOT_CLASS(Voter, godot::Object)

private:
    float loyalty = 50.0f; // Influence by player/actions
    float independence = 50.0f; // Tendency to follow stats vs. player influence

public:
    static void _register_methods();

    Voter();
    ~Voter();

    void _init(); // Called by Godot

    void set_loyalty(float value);
    float get_loyalty() const;

    void set_independence(float value);
    float get_independence() const;

    bool supports(Candidate *candidate, Region *region);
};

} // namespace Systems

#endif // VOTER_H 