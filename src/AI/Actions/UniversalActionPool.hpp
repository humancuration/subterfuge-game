#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <random>

class UniversalActionPool {
public:
    struct UniversalAction {
        std::string id;
        std::string name;
        
        enum class Category {
            TOOL_USE,           // Using/creating tools
            AGRICULTURE,        // Growing/tending resources
            CONSTRUCTION,       // Building/modifying environment
            COMMUNICATION,      // Complex information sharing
            SOCIAL_ORGANIZATION, // Group coordination
            ENVIRONMENTAL,      // Ecosystem interaction
            TECHNOLOGICAL,      // Advanced tool/technique use
            CULTURAL,          // Symbolic/ritual behavior
            TEACHING,          // Knowledge transfer
            INNOVATION         // Creating new techniques
        } category;

        // Requirements for action discovery/learning
        struct Prerequisites {
            float cognitive_complexity{0.0f};  // Required brain power
            float physical_capability{0.0f};   // Required physical traits
            float social_complexity{0.0f};     // Required social organization
            float technological_level{0.0f};   // Required tech understanding
            std::vector<std::string> required_traits;
        } prerequisites;

        // How this action can spread
        struct Transmission {
            bool can_be_observed{true};     // Can be learned by watching
            bool requires_teaching{false};   // Needs direct instruction
            bool can_be_innovated{true};    // Can be discovered independently
            float learning_difficulty{0.5f}; // How hard to learn
        } transmission;
    };

private:
    std::unordered_map<std::string, UniversalAction> action_pool;
    
    // Track which species have discovered which actions
    std::unordered_map<std::string, std::vector<std::string>> species_actions;
    
    // Track historical first discoveries
    struct Discovery {
        std::string action_id;
        std::string species_id;
        float timestamp;
        bool was_taught{false};
    };
    std::vector<Discovery> discovery_history;

    struct Capabilities {
        float cognitive;
        float physical;
        float social;
        float tech;
    };

    Capabilities get_species_capabilities(const std::string& species_id) {
        // TODO: Implement actual capability lookup
        return {0.5f, 0.5f, 0.5f, 0.5f}; // Default values
    }

    void add_action(const UniversalAction& action) {
        action_pool[action.id] = action;
    }

    bool has_required_traits(const std::string& species_id, 
                           const std::vector<std::string>& required_traits) {
        // TODO: Implement actual trait checking
        return required_traits.empty(); // For now, only pass if no traits required
    }

    float random_float() {
        // Create static random number generator for reuse
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        
        return dist(gen);
    }

public:
    void initialize_action_pool() {
        // Tool use actions
        add_action({
            "use_simple_tool",
            "Use Simple Tool",
            UniversalAction::Category::TOOL_USE,
            {0.3f, 0.4f, 0.1f, 0.2f},  // Relatively basic requirements
            {true, false, true, 0.3f}   // Easy to learn by observation
        });

        add_action({
            "create_complex_tool",
            "Create Complex Tool",
            UniversalAction::Category::TOOL_USE,
            {0.6f, 0.7f, 0.4f, 0.5f},  // More advanced requirements
            {true, true, true, 0.7f}    // Harder to learn
        });

        // Agricultural actions
        add_action({
            "tend_plants",
            "Tend Plants",
            UniversalAction::Category::AGRICULTURE,
            {0.4f, 0.3f, 0.5f, 0.3f},
            {true, false, true, 0.4f}
        });

        // Social actions
        add_action({
            "coordinate_group",
            "Coordinate Group",
            UniversalAction::Category::SOCIAL_ORGANIZATION,
            {0.5f, 0.2f, 0.7f, 0.3f},
            {true, true, false, 0.6f}
        });

        // Environmental actions
        add_action({
            "modify_habitat",
            "Modify Habitat",
            UniversalAction::Category::ENVIRONMENTAL,
            {0.5f, 0.6f, 0.4f, 0.4f},
            {true, false, true, 0.5f}
        });
    }

    bool can_species_learn_action(const std::string& species_id, 
                                const std::string& action_id) {
        const auto& action = action_pool[action_id];
        const auto& prereq = action.prerequisites;
        
        // Get species capabilities
        auto capabilities = get_species_capabilities(species_id);
        
        // Check if species meets requirements
        return capabilities.cognitive >= prereq.cognitive_complexity &&
               capabilities.physical >= prereq.physical_capability &&
               capabilities.social >= prereq.social_complexity &&
               capabilities.tech >= prereq.technological_level &&
               has_required_traits(species_id, prereq.required_traits);
    }

    bool attempt_action_discovery(const std::string& species_id,
                                const std::string& action_id) {
        if (!can_species_learn_action(species_id, action_id)) {
            return false;
        }

        const auto& action = action_pool[action_id];
        
        // Calculate discovery chance
        float base_chance = calculate_discovery_chance(species_id, action);
        
        // Roll for discovery
        if (random_float() < base_chance) {
            record_discovery(species_id, action_id);
            return true;
        }
        
        return false;
    }

    bool attempt_action_learning(const std::string& learner_id,
                               const std::string& teacher_id,
                               const std::string& action_id) {
        if (!can_species_learn_action(learner_id, action_id)) {
            return false;
        }

        const auto& action = action_pool[action_id];
        
        // Calculate learning chance
        float base_chance = calculate_learning_chance(
            learner_id, teacher_id, action
        );
        
        // Roll for learning
        if (random_float() < base_chance) {
            record_discovery(learner_id, action_id, true);
            return true;
        }
        
        return false;
    }

    float calculate_discovery_chance(const std::string& species_id, 
                                   const UniversalAction& action) {
        auto capabilities = get_species_capabilities(species_id);
        
        // Base chance affected by species capabilities vs requirements
        float base_chance = std::min({
            capabilities.cognitive / action.prerequisites.cognitive_complexity,
            capabilities.physical / action.prerequisites.physical_capability,
            capabilities.social / action.prerequisites.social_complexity,
            capabilities.tech / action.prerequisites.technological_level
        });

        // Modify by transmission properties
        if (!action.transmission.can_be_innovated) {
            return 0.0f;
        }
        
        return base_chance / action.transmission.learning_difficulty;
    }

    float calculate_learning_chance(const std::string& learner_id,
                                  const std::string& teacher_id,
                                  const UniversalAction& action) {
        if (!action.transmission.can_be_observed) {
            return 0.0f;
        }

        float base_chance = calculate_discovery_chance(learner_id, action);
        
        // Teaching makes learning easier
        if (action.transmission.requires_teaching) {
            base_chance *= 2.0f;
        }
        
        return base_chance;
    }

private:
    void record_discovery(const std::string& species_id, 
                         const std::string& action_id,
                         bool was_taught = false) {
        species_actions[species_id].push_back(action_id);
        
        discovery_history.push_back({
            action_id,
            species_id,
            get_current_time(), // You'll need to implement this
            was_taught
        });
    }
}; 