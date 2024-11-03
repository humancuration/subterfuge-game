#ifndef HEALTHSYSTEM_HPP
#define HEALTHSYSTEM_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include <RandomNumberGenerator.hpp>
#include "ISystem.hpp"
#include "../Models/Node.hpp"
#include "../Events/EventManager.hpp"
#include <vector>

namespace Systems {

class HealthSystem : public godot::Node, public ISystem {
    GODOT_CLASS(HealthSystem, godot::Node)

private:
    std::vector<Node*> nodes;
    godot::Ref<godot::RandomNumberGenerator> rng;

    struct DiseaseOutbreak {
        godot::String name;
        float severity;
        float spread_rate;
        float duration;
    };

    std::vector<DiseaseOutbreak> active_outbreaks;

public:
    static void _register_methods();
    
    HealthSystem();
    ~HealthSystem();

    void _init();
    void set_nodes(const std::vector<Node*>& node_list);
    void update(float delta_time) override;

private:
    void update_health(Node* node, float delta_time);
    void trigger_health_event(Node* node);
    void update_disease_spread(float delta_time);
    float calculate_infection_risk(Node* node) const;
    void apply_healthcare_measures(Node* node, float delta_time);
};

} // namespace Systems

#endif // HEALTHSYSTEM_HPP 