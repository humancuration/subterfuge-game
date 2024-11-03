#include "HealthSystem.hpp"
#include <Math.hpp>

namespace Systems {

using namespace godot;

void HealthSystem::_register_methods() {
    register_method("_init", &HealthSystem::_init);
    register_method("update", &HealthSystem::update);
    register_method("set_nodes", &HealthSystem::set_nodes);
}

HealthSystem::HealthSystem() {
    rng.instance();
    rng->randomize();
}

HealthSystem::~HealthSystem() {}

void HealthSystem::_init() {}

void HealthSystem::set_nodes(const std::vector<Node*>& node_list) {
    nodes = node_list;
}

void HealthSystem::update(float delta_time) {
    for (auto* node : nodes) {
        update_health(node, delta_time);
        apply_healthcare_measures(node, delta_time);
    }
    update_disease_spread(delta_time);
}

void HealthSystem::update_health(Node* node, float delta_time) {
    float health_change = delta_time * (
        -node->get_stats().environmental_health * 0.05f + 
        node->get_stats().medical_resources * 0.1f
    );
    
    // Consider population density impact
    health_change -= node->get_stats().population_density * 0.02f * delta_time;
    
    node->modify_health_risk(health_change);
    
    // Random health events
    if (rng->randf() < 0.003f) {
        trigger_health_event(node);
    }
}

void HealthSystem::trigger_health_event(Node* node) {
    DiseaseOutbreak outbreak{
        "Viral Outbreak",
        rng->randf_range(0.5f, 1.5f),
        rng->randf_range(0.1f, 0.3f),
        rng->randf_range(100.0f, 300.0f)
    };
    
    active_outbreaks.push_back(outbreak);
    
    // Create and trigger the event
    GameEvent* event = GameEvent::_new();
    event->set_name("Disease Outbreak");
    event->set_description("A disease outbreak has occurred in " + node->get_name());
    
    EventManager::get_instance()->trigger_event(event);
}

void HealthSystem::update_disease_spread(float delta_time) {
    for (auto it = active_outbreaks.begin(); it != active_outbreaks.end();) {
        it->duration -= delta_time;
        
        if (it->duration <= 0) {
            it = active_outbreaks.erase(it);
        } else {
            for (auto* node : nodes) {
                float infection_risk = calculate_infection_risk(node);
                if (rng->randf() < infection_risk * it->spread_rate * delta_time) {
                    node->modify_health_risk(10.0f * it->severity);
                }
            }
            ++it;
        }
    }
}

float HealthSystem::calculate_infection_risk(Node* node) const {
    float base_risk = node->get_stats().population_density / 100.0f;
    float medical_factor = 1.0f - (node->get_stats().medical_resources / 100.0f);
    return base_risk * medical_factor;
}

void HealthSystem::apply_healthcare_measures(Node* node, float delta_time) {
    if (node->get_stats().health_risk > 50.0f) {
        // Emergency healthcare measures
        float recovery_rate = node->get_stats().medical_resources * 0.2f * delta_time;
        node->modify_health_risk(-recovery_rate);
    }
}

} // namespace Systems 