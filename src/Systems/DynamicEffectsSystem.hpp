#ifndef DYNAMICEFFECTSSYSTEM_HPP
#define DYNAMICEFFECTSSYSTEM_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include "ISystem.hpp"
#include "../Core/GameState.hpp"
#include <vector>
#include <functional>

namespace Systems {

class DynamicEffectsSystem : public godot::Node, public ISystem {
    GODOT_CLASS(DynamicEffectsSystem, godot::Node)

private:
    struct DynamicEffect {
        godot::String name;
        float duration;
        float intensity;
        std::function<void(float, float)> apply_effect;
    };

    std::vector<DynamicEffect> active_effects;
    godot::Ref<godot::RandomNumberGenerator> rng;

public:
    static void _register_methods();
    
    DynamicEffectsSystem();
    ~DynamicEffectsSystem();

    void _init();
    void update(float delta_time) override;

    void add_effect(const godot::String& name, float duration, float intensity, 
                   std::function<void(float, float)> effect);
    void remove_effect(const godot::String& name);

private:
    void update_traffic_effects(float delta_time);
    void update_pollution_effects(float delta_time);
    void update_economy_effects(float delta_time);
    void update_research_effects(float delta_time);
    void update_active_effects(float delta_time);
    void check_emergent_effects(float delta_time);
};

} // namespace Systems

#endif // DYNAMICEFFECTSSYSTEM_HPP 