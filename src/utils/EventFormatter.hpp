#pragma once
#include <string>
#include <vector>
#include "../Core/Pattern.hpp"

class EventFormatter {
public:
    static std::string format_discovery_event(
        const std::string& species_name,
        const Pattern& discovered_pattern) {
        
        std::string text = species_name + " has discovered ";
        text += discovered_pattern.name + "!\n\n";
        
        // Add details about the discovery
        text += "Through ";
        text += join_with_commas(discovered_pattern.actions);
        text += ", they have learned new capabilities.\n\n";
        
        // Add effects
        if (!discovered_pattern.cultural_elements.empty()) {
            text += "Cultural Impact:\n";
            text += "• " + join_with_bullets(discovered_pattern.cultural_elements);
            text += "\n";
        }
        
        return text;
    }

    static std::string format_interaction_event(
        const std::string& species1,
        const std::string& species2,
        const std::string& interaction_type) {
        
        return species1 + " and " + species2 + 
               " are engaging in " + interaction_type + ".\n";
    }

private:
    static std::string join_with_commas(
        const std::vector<std::string>& items) {
        std::string result;
        for (size_t i = 0; i < items.size(); ++i) {
            if (i > 0) {
                result += i == items.size() - 1 ? " and " : ", ";
            }
            result += items[i];
        }
        return result;
    }

    static std::string join_with_bullets(
        const std::vector<std::string>& items) {
        std::string result;
        for (const auto& item : items) {
            result += "• " + item + "\n";
        }
        return result;
    }
}; 