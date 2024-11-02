#ifndef TECHNOLOGYSYSTEM_H
#define TECHNOLOGYSYSTEM_H

#include <Godot.hpp>
#include <Node.hpp>
#include "Models/Node.h"

namespace Systems {

class TechnologySystem : public godot::Node {
    GODOT_CLASS(TechnologySystem, godot::Node)

private:
    godot::Array nodes; // Assuming nodes are instances of a Node class

public:
    static void _register_methods();

    TechnologySystem();
    ~TechnologySystem();

    void _init(); // Called by Godot
    void update_system(float delta_time);
    void update_technology(int node_index, float delta_time);
    void trigger_tech_breakthrough(int node_index);
};

} // namespace Systems

#endif // TECHNOLOGYSYSTEM_H 