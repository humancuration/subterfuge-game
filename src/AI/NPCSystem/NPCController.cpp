#include "NPCController.hpp"
#include <godot_cpp/core/class_db.hpp>

void NPCController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("update", "delta"), &NPCController::update);
    ClassDB::bind_method(D_METHOD("get_cultural_identity"), &NPCController::get_cultural_identity);
    ClassDB::bind_method(D_METHOD("get_species_identity"), &NPCController::get_species_identity);
}

void NPCController::process_behavior(float delta_time) {
    // Update behavior context
    BehaviorContext context{
        .delta_time = delta_time,
        .environmental_awareness = environmental_awareness,
        .resource_efficiency = resource_efficiency,
        .cultural_receptivity = cultural_identity->get_receptivity()
    };

    // Execute behavior tree with context
    if (behavior_tree) {
        behavior_tree->execute(this);
    }

    // Process learning and adaptation
    process_learning(delta_time);
}

void NPCController::process_learning(float delta_time) {
    // Learn from environment
    float learning_rate = species_identity->get_trait("Learning_Rate");
    
    // Environmental learning
    if (is_experiencing_environmental_stress()) {
        environmental_awareness = std::min(
            1.0f,
            environmental_awareness + learning_rate * delta_time
        );
    }
    
    // Cultural learning
    if (is_in_cultural_exchange()) {
        cultural_identity->increase_receptivity(learning_rate * delta_time);
    }
    
    // Resource efficiency learning
    if (resource_network) {
        float efficiency_gain = resource_network->calculate_efficiency_learning(
            this,
            learning_rate,
            delta_time
        );
        resource_efficiency = std::min(1.0f, resource_efficiency + efficiency_gain);
    }
}

void NPCController::share_resources_and_knowledge(float delta_time) {
    if (resource_network) {
        // Calculate sharing amount based on cultural values and species traits
        float sharing_amount = calculate_sharing_amount();
        
        if (sharing_amount > 0) {
            auto nearby_npcs = get_nearby_npcs();
            for (auto* other : nearby_npcs) {
                if (should_share_with(other)) {
                    resource_network->transfer_resources(
                        this,
                        other,
                        sharing_amount
                    );
                }
            }
        }
    }

    if (knowledge_network) {
        // Share knowledge based on cultural practices
        auto knowledge = get_sharable_knowledge();
        if (!knowledge.empty()) {
            knowledge_network->share_knowledge(
                this,
                get_nearby_npcs(),
                knowledge
            );
        }
    }
} 