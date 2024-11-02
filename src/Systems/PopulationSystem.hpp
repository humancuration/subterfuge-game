#ifndef POPULATIONSYSTEM_HPP
#define POPULATIONSYSTEM_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include <PackedScene.hpp>
#include <RandomNumberGenerator.hpp>
#include "ISystem.hpp"

namespace Systems {

class PopulationSystem : public godot::Node, public ISystem {
    GODOT_CLASS(PopulationSystem, godot::Node)

private:
    godot::Ref<godot::PackedScene> tree_prefab;
    godot::Ref<godot::PackedScene> building_prefab;
    godot::Ref<godot::RandomNumberGenerator> rng;

public:
    static void _register_methods();
    
    PopulationSystem();
    ~PopulationSystem();

    void _init();
    void _ready();
    
    void update(float delta_time) override;
    void populate_area(const godot::Vector3& position, const godot::String& type);
    void populate_multiple_areas();
};

} // namespace Systems

#endif // POPULATIONSYSTEM_HPP 