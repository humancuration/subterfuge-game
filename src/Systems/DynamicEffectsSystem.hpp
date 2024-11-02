#ifndef DYNAMICEFFECTSSYSTEM_HPP
#define DYNAMICEFFECTSSYSTEM_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include "ISystem.hpp"

namespace Systems {

class DynamicEffectsSystem : public godot::Node, public ISystem {
    GODOT_CLASS(DynamicEffectsSystem, godot::Node)

public:
    static void _register_methods();
    
    DynamicEffectsSystem();
    ~DynamicEffectsSystem();

    void _init();
    void update(float delta_time) override;

private:
    void update_traffic_effects(float delta_time);
    void update_pollution_effects(float delta_time);
    void update_economy_effects(float delta_time);
    void update_research_effects(float delta_time);
};

} // namespace Systems

#endif // DYNAMICEFFECTSSYSTEM_HPP 