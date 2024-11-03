#include "DynamicEffectsSystem.hpp"
#include <Math.hpp>
#include <algorithm>

namespace Systems {

using namespace godot;

void DynamicEffectsSystem::_register_methods() {
    register_method("update", &DynamicEffectsSystem::update);
    register_method("add_effect", &DynamicEffectsSystem::add_effect);
    register_method("remove_effect", &DynamicEffectsSystem::remove_effect);
}

DynamicEffectsSystem::DynamicEffectsSystem() {
    rng.instance();
    rng->randomize();
}

DynamicEffectsSystem::~DynamicEffectsSystem() {}

void DynamicEffectsSystem::_init() {}

void DynamicEffectsSystem::update(float delta_time) {
    update_traffic_effects(delta_time);
    update_pollution_effects(delta_time);
    update_economy_effects(delta_time);
    update_research_effects(delta_time);
    update_active_effects(delta_time);
    check_emergent_effects(delta_time);
}

void DynamicEffectsSystem::add_effect(const String& name, float duration, float intensity,
                                    std::function<void(float, float)> effect) {
    DynamicEffect new_effect{name, duration, intensity, effect};
    active_effects.push_back(new_effect);
}

void DynamicEffectsSystem::remove_effect(const String& name) {
    active_effects.erase(
        std::remove_if(active_effects.begin(), active_effects.end(),
            [&name](const DynamicEffect& effect) { return effect.name == name; }),
        active_effects.end()
    );
}

void DynamicEffectsSystem::update_active_effects(float delta_time) {
    for (auto it = active_effects.begin(); it != active_effects.end();) {
        it->duration -= delta_time;
        if (it->duration <= 0) {
            it = active_effects.erase(it);
        } else {
            it->apply_effect(delta_time, it->intensity);
            ++it;
        }
    }
}

void DynamicEffectsSystem::check_emergent_effects(float delta_time) {
    auto* game_state = GameState::get_instance();
    
    // Check for environmental crisis
    if (game_state->get_air_pollution() > 75.0f && 
        game_state->get_public_health() < 40.0f) {
        add_effect("environmental_crisis", 300.0f, 1.5f,
            [](float dt, float intensity) {
                GameState::get_instance()->modify_public_health(-0.2f * intensity * dt);
                GameState::get_instance()->modify_economic_productivity(-0.15f * intensity * dt);
            });
    }

    // Check for economic boom
    if (game_state->get_economic_productivity() > 80.0f && 
        game_state->get_public_research_progress() > 70.0f) {
        add_effect("economic_boom", 200.0f, 1.2f,
            [](float dt, float intensity) {
                GameState::get_instance()->modify_economic_productivity(0.1f * intensity * dt);
                GameState::get_instance()->modify_public_welfare(0.05f * intensity * dt);
            });
    }
}

void DynamicEffectsSystem::update_traffic_effects(float delta_time) {
    float traffic_reduction = GameState::get_instance()->get_public_transport_usage() * 0.5f;
    GameState::get_instance()->set_traffic_congestion(
        Math::max(0.0f, GameState::get_instance()->get_traffic_congestion() - traffic_reduction)
    );
    GameState::get_instance()->set_air_pollution(
        Math::max(0.0f, GameState::get_instance()->get_air_pollution() - (traffic_reduction * 0.3f))
    );
}

void DynamicEffectsSystem::update_pollution_effects(float delta_time) {
    if (GameState::get_instance()->get_air_pollution() > 50.0f) {
        GameState::get_instance()->modify_public_health(-0.1f * delta_time);
        GameState::get_instance()->modify_tourism_revenue(-100.0f * delta_time);
    }
}

void DynamicEffectsSystem::update_economy_effects(float delta_time) {
    float congestion = GameState::get_instance()->get_traffic_congestion();
    if (congestion < 30.0f) {
        GameState::get_instance()->modify_economic_productivity(0.05f * delta_time);
    } else if (congestion > 70.0f) {
        GameState::get_instance()->modify_economic_productivity(-0.1f * delta_time);
    }
}

void DynamicEffectsSystem::update_research_effects(float delta_time) {
    float public_research = GameState::get_instance()->get_public_research_progress();
    float corporate_research = GameState::get_instance()->get_corporate_research_progress();
    float public_ratio = public_research / (public_research + corporate_research);

    if (public_ratio > 0.6f) {
        GameState::get_instance()->modify_public_welfare(0.1f * delta_time);
        GameState::get_instance()->modify_wealth_inequality(-0.05f * delta_time);
    } else if (public_ratio < 0.4f) {
        GameState::get_instance()->modify_economic_productivity(0.15f * delta_time);
        GameState::get_instance()->modify_wealth_inequality(0.1f * delta_time);
    }
}

} // namespace Systems 