#include "DynamicEffectsSystem.hpp"
#include <Math.hpp>

namespace Systems {

using namespace godot;

void DynamicEffectsSystem::_register_methods() {
    register_method("update", &DynamicEffectsSystem::update);
}

DynamicEffectsSystem::DynamicEffectsSystem() {}
DynamicEffectsSystem::~DynamicEffectsSystem() {}

void DynamicEffectsSystem::_init() {}

void DynamicEffectsSystem::update(float delta_time) {
    update_traffic_effects(delta_time);
    update_pollution_effects(delta_time);
    update_economy_effects(delta_time);
    update_research_effects(delta_time);
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