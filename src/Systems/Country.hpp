#ifndef COUNTRY_H
#define COUNTRY_H

#include <Godot.hpp>
#include <Object.hpp>
#include <String.hpp>
#include <Array.hpp>
#include <Vector.hpp>

#include "Region.h"
#include "Election.h"

namespace Systems {

class Country : public godot::Object {
    GODOT_CLASS(Country, godot::Object)

private:
    godot::String name;
    godot::Array regions; // Array of Region instances
    godot::Array national_elections; // Array of Election instances

public:
    static void _register_methods();

    Country();
    ~Country();

    void _init(); // Called by Godot

    void initialize_regions();
    void initialize_elections();
    void handle_elections();
    void conduct_election(int election_index);
    godot::Array load_candidates(const godot::String &type);
};

} // namespace Systems

#endif // COUNTRY_H 