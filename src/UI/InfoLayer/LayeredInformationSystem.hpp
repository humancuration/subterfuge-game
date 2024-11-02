#pragma once
#include <godot_cpp/classes/control.hpp>
#include "../../Utils/UIHelper.hpp"

class LayeredInformationSystem {
public:
    // Different detail levels for information
    enum class DetailLevel {
        QUICK_TOOLTIP,    // One-liner explanation
        BASIC,           // Simple paragraph
        DETAILED,        // Full mechanics explanation
        EDUCATIONAL      // Real-world connections/learning
    };

    struct InfoContent {
        std::string quick_tip;
        std::string basic_description;
        std::string detailed_explanation;
        struct Educational {
            std::string real_world_example;
            std::string historical_context;
            std::string further_reading;
        } educational;
    };

    // Example for voting systems
    static InfoContent get_voting_system_info(const std::string& system_name) {
        if (system_name == "consensus") {
            return {
                "Everyone must agree", // Quick tooltip
                
                "A decision-making process where all members need to agree before moving forward.", // Basic
                
                "Consensus decision-making requires all group members to agree or give consent. "
                "Members can voice concerns and blocks, leading to proposal modifications until "
                "everyone can accept the decision.", // Detailed
                
                {
                    "Used by many indigenous peoples and modern cooperatives", // Real example
                    "Traditional consensus processes in the Haudenosaunee Confederacy", // Historical
                    "Look up: Sociocracy, Indigenous Decision-Making" // Further reading
                }
            };
        }
        // Add other voting systems...
        return {};
    }

    // Example for environmental interactions
    static InfoContent get_environmental_info(const std::string& interaction_type) {
        if (interaction_type == "mycelial_network") {
            return {
                "Underground fungal communication network", // Quick tooltip
                
                "Plants share resources and information through underground fungal connections.", // Basic
                
                "Mycelial networks allow plants to transfer nutrients, send warning signals about "
                "threats, and support younger plants. Also called the 'Wood Wide Web'.", // Detailed
                
                {
                    "Pacific Northwest forest fungal networks", // Real example
                    "First observed by Dr. Suzanne Simard", // Historical
                    "Research: Forest Ecology, Mycorrhizal Networks" // Further reading
                }
            };
        }
        return {};
    }

    // Make it fun and engaging
    static std::string get_fun_fact(const std::string& topic) {
        static const std::unordered_map<std::string, std::string> fun_facts = {
            {"consensus", "Some ant colonies use a form of consensus to decide on new nest locations!"},
            {"mycelial_network", "A single fungal network can connect hundreds of trees - nature's internet!"},
            {"liquid_democracy", "Like choosing the best pizza toppings by letting your foodie friend decide!"}
        };
        return fun_facts.contains(topic) ? fun_facts.at(topic) : "";
    }

    // Get appropriate emoji/icon
    static std::string get_topic_icon(const std::string& topic) {
        static const std::unordered_map<std::string, std::string> icons = {
            {"consensus", "🤝"},
            {"mycelial_network", "🍄"},
            {"liquid_democracy", "🌊"},
            {"ranked_choice", "📊"},
            {"environmental", "🌿"}
        };
        return icons.contains(topic) ? icons.at(topic) : "ℹ️";
    }
}; 