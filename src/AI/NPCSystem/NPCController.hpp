#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include <vector>
#include <memory>
#include "../Characters/CulturalIdentity.hpp"
#include "../Map/SpeciesIdentity.hpp"
#include "../Map/ResourceSharingNetwork.hpp"
#include "../Map/KnowledgeSharingSystem.hpp"

class NPCController : public godot::Node3D {
    GDCLASS(NPCController, Node3D)

public:
    // Public interface for behaviors to use
    std::vector<NPCController*> get_nearby_npcs(float radius = 10.0f) const;
    CulturalIdentity* get_cultural_identity() const { return cultural_identity.get(); }
    SpeciesIdentity* get_species_identity() const { return species_identity.get(); }
    
    // Knowledge sharing
    bool share_knowledge(NPCController* recipient, const Knowledge& knowledge);
    std::vector<Knowledge> get_sharable_knowledge() const;
    
    // Resource management
    float get_resource_consumption() const { return resource_consumption; }
    void reduce_resource_consumption(float amount = 0.1f);
    void adopt_sustainable_practice();
    
    // Environmental interaction
    EnvironmentalState get_local_environment() const;
    bool is_experiencing_environmental_stress() const;
    bool is_in_cultural_exchange() const;

private:
    // Core components
    std::unique_ptr<CulturalIdentity> cultural_identity;
    std::shared_ptr<SpeciesIdentity> species_identity;
    std::unique_ptr<BehaviorNode> behavior_tree;
    
    // Network connections
    ResourceSharingNetwork* resource_network{nullptr};
    KnowledgeSharingSystem* knowledge_network{nullptr};
    
    // State tracking
    float environmental_awareness{0.5f};
    float resource_consumption{0.5f};
    float resource_efficiency{0.5f};
    
    // Internal methods
    void process_behavior(float delta_time);
    void process_learning(float delta_time);
    void share_resources_and_knowledge(float delta_time);
    float calculate_sharing_amount() const;
    bool should_share_with(NPCController* other) const;

protected:
    static void _bind_methods();
}; 