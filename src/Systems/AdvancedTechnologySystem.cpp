#include "AdvancedTechnologySystem.hpp"
#include "../Utils/Random.hpp"
#include "../Core/GameState.hpp"
#include <algorithm>

void AdvancedTechnologySystem::initialize() {
    emergence_system = std::make_unique<EmergentTechnologySystem>();
    pattern_system = std::make_unique<PatternRecognitionSystem>();
    
    emergence_system->initialize_potential_discoveries();
}

void AdvancedTechnologySystem::process_crisis_innovations() {
    for (auto& advancement : advancements) {
        if (advancement.second.type == TechnologyAdvancement::AdvancementType::CRISIS_INNOVATION) {
            float crisis_pressure = calculate_crisis_pressure(advancement.second);
            float innovation_chance = crisis_pressure * advancement.second.factors.knowledge_base;
            
            if (Random::get_float() < innovation_chance) {
                trigger_crisis_discovery(advancement.second);
            }
        }
    }
}

void AdvancedTechnologySystem::update_research_factors(TechnologyAdvancement& advancement) {
    // Update cultural readiness based on contributing species
    advancement.factors.cultural_readiness = calculate_cultural_readiness(
        advancement.contributing_species
    );
    
    // Update resource availability based on funding and global resources
    advancement.factors.resource_availability = calculate_resource_availability(
        advancement.funding,
        GameState::get_instance().get_global_resources()
    );
    
    // Update environmental pressure
    advancement.factors.environmental_pressure = calculate_environmental_pressure(
        advancement.tech_id
    );
    
    // Update knowledge base
    advancement.factors.knowledge_base = calculate_knowledge_base(
        advancement.tech_id,
        advancement.contributing_species
    );
}

std::vector<std::string> AdvancedTechnologySystem::get_cultural_elements(
    const std::string& species_id) {
    std::vector<std::string> elements;
    
    // Get cultural practices from species
    auto species = GameState::get_instance().get_species(species_id);
    for (const auto& practice : species.cultural_practices) {
        elements.push_back(practice.id);
    }
    
    // Add any technological traditions
    auto tech_traditions = get_species_tech_traditions(species_id);
    elements.insert(elements.end(), 
                   tech_traditions.begin(), 
                   tech_traditions.end());
    
    return elements;
}

void AdvancedTechnologySystem::trigger_discovery(TechnologyAdvancement& advancement) {
    // Record the discovery
    auto& species = GameState::get_instance().get_species(
        advancement.contributing_species[0]
    );
    species.discovered_technologies.push_back(advancement.tech_id);
    
    // Apply immediate effects
    apply_discovery_effects(advancement);
    
    // Notify observers
    notify_discovery(advancement);
    
    // Update global state
    update_global_tech_state(advancement);
}

void AdvancedTechnologySystem::complete_research(TechnologyAdvancement& advancement) {
    // Apply research completion effects
    apply_research_completion(advancement);
    
    // Distribute benefits based on funding contributions
    distribute_research_benefits(advancement);
    
    // Update global knowledge
    update_global_knowledge(advancement);
    
    // Trigger potential follow-up research
    generate_follow_up_research(advancement);
}

float AdvancedTechnologySystem::calculate_crisis_pressure(
    const TechnologyAdvancement& advancement) {
    float pressure = 0.0f;
    
    // Environmental crisis pressure
    pressure += GameState::get_instance().get_environmental_crisis_level();
    
    // Resource scarcity pressure
    pressure += calculate_resource_scarcity_pressure(advancement.tech_id);
    
    // Social pressure
    pressure += calculate_social_pressure(advancement.contributing_species);
    
    return std::clamp(pressure, 0.0f, 1.0f);
}

void AdvancedTechnologySystem::trigger_crisis_discovery(
    TechnologyAdvancement& advancement) {
    // Crisis discoveries get bonus effects
    advancement.factors.environmental_pressure *= 1.5f;
    advancement.progress += 0.2f; // Crisis boost
    
    // Normal discovery process
    trigger_discovery(advancement);
    
    // Additional crisis-specific effects
    apply_crisis_discovery_effects(advancement);
}

float AdvancedTechnologySystem::calculate_cultural_readiness(
    const std::vector<std::string>& species_ids) {
    float total_readiness = 0.0f;
    
    for (const auto& species_id : species_ids) {
        auto cultural_factors = get_species_cultural_factors(species_id);
        total_readiness += evaluate_cultural_readiness(cultural_factors);
    }
    
    return total_readiness / species_ids.size();
}

float AdvancedTechnologySystem::calculate_resource_availability(
    float funding, const Resources& global_resources) {
    // Base availability from funding
    float availability = funding / 100.0f; // Normalize to 0-1
    
    // Modify based on global resource levels
    availability *= global_resources.get_abundance_factor();
    
    // Consider infrastructure
    availability *= GameState::get_instance().get_infrastructure_factor();
    
    return std::clamp(availability, 0.0f, 1.0f);
}

void AdvancedTechnologySystem::apply_discovery_effects(
    const TechnologyAdvancement& advancement) {
    // Get discovery template
    auto discovery = emergence_system->get_discovery(advancement.tech_id);
    
    // Apply capability changes
    for (const auto& species_id : advancement.contributing_species) {
        auto& species = GameState::get_instance().get_species(species_id);
        
        // Grant new actions
        for (const auto& action : discovery.capabilities.new_actions) {
            species.available_actions.push_back(action);
        }
        
        // Enhance abilities
        for (const auto& ability : discovery.capabilities.enhanced_abilities) {
            species.enhance_ability(ability);
        }
        
        // Update complexity
        species.increase_complexity(discovery.capabilities.complexity_increase);
    }
}

void AdvancedTechnologySystem::notify_discovery(
    const TechnologyAdvancement& advancement) {
    // Notify UI
    GameState::get_instance().get_ui_manager().show_discovery_notification(
        advancement.tech_id,
        advancement.contributing_species
    );
    
    // Log discovery
    Logger::log_discovery(advancement);
    
    // Notify other systems
    notify_observers(AdvancementEvent::DISCOVERY, advancement);
} 