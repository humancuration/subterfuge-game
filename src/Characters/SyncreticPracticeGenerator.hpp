#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class SyncreticPracticeGenerator {
public:
    struct CulturalElement {
        enum class Type {
            RITUAL,         // Ceremonies and practices
            PHILOSOPHY,     // Beliefs and worldviews
            TRADITION,      // Inherited customs
            TECHNOLOGY,     // Tools and techniques
            ART,           // Creative expression
            SOCIAL         // Community organization
        };

        Type type;
        std::string name;
        float adaptability{0.5f};      // How easily it combines with others
        float core_importance{1.0f};   // How central it is to the culture
        std::vector<std::string> compatible_elements;
    };

    struct SyncreticPractice {
        std::string name;
        std::vector<std::string> parent_elements;
        float stability{1.0f};         // How well the combination works
        float innovation_level{0.0f};  // How much new emerged from combination
        bool is_sustainable{true};
    };

private:
    // Keep track of successful combinations
    struct CombinationMemory {
        std::string practice_name;
        float success_rating;
        std::vector<std::string> key_insights;
    };

    std::unordered_map<std::string, CulturalElement> cultural_database;
    std::vector<CombinationMemory> successful_combinations;

public:
    SyncreticPractice generate_practice(
        const CulturalElement& element1,
        const CulturalElement& element2) {
        
        // Check basic compatibility
        float compatibility = calculate_compatibility(element1, element2);
        
        if (compatibility > 0.5f) {
            // Generate new practice name
            std::string practice_name = create_practice_name(element1, element2);
            
            // Calculate stability based on elements
            float stability = calculate_stability(element1, element2);
            
            // Determine innovation level
            float innovation = calculate_innovation_level(element1, element2);
            
            // Create new practice
            SyncreticPractice practice{
                practice_name,
                {element1.name, element2.name},
                stability,
                innovation,
                true
            };
            
            // Record if successful
            if (stability > 0.7f) {
                record_successful_combination(practice);
            }
            
            return practice;
        }
        
        return {"Failed_Combination", {}, 0.0f, 0.0f, false};
    }

private:
    float calculate_compatibility(const CulturalElement& e1, 
                                const CulturalElement& e2) {
        // Base compatibility on type relationship
        float type_compatibility = get_type_compatibility(e1.type, e2.type);
        
        // Adjust for adaptability
        float adaptability_factor = (e1.adaptability + e2.adaptability) / 2.0f;
        
        // Check explicit compatibility
        bool explicitly_compatible = 
            std::find(e1.compatible_elements.begin(),
                     e1.compatible_elements.end(),
                     e2.name) != e1.compatible_elements.end();
        
        return type_compatibility * adaptability_factor * 
               (explicitly_compatible ? 1.2f : 1.0f);
    }

    float get_type_compatibility(CulturalElement::Type t1, 
                               CulturalElement::Type t2) {
        // Define how well different types combine
        static const float compatibility_matrix[6][6] = {
            // RITUAL  PHIL    TRAD    TECH    ART     SOCIAL
            {1.0f,    0.8f,   0.9f,   0.5f,   0.7f,   0.8f},  // RITUAL
            {0.8f,    1.0f,   0.7f,   0.6f,   0.8f,   0.9f},  // PHILOSOPHY
            {0.9f,    0.7f,   1.0f,   0.5f,   0.6f,   0.8f},  // TRADITION
            {0.5f,    0.6f,   0.5f,   1.0f,   0.7f,   0.6f},  // TECHNOLOGY
            {0.7f,    0.8f,   0.6f,   0.7f,   1.0f,   0.8f},  // ART
            {0.8f,    0.9f,   0.8f,   0.6f,   0.8f,   1.0f}   // SOCIAL
        };
        
        return compatibility_matrix[static_cast<int>(t1)][static_cast<int>(t2)];
    }

    std::string create_practice_name(const CulturalElement& e1,
                                   const CulturalElement& e2) {
        // Create meaningful combination names based on type
        switch (e1.type) {
            case CulturalElement::Type::RITUAL:
                return "Harmonized_" + e1.name + "_" + e2.name;
            case CulturalElement::Type::PHILOSOPHY:
                return "Unified_" + e1.name + "_" + e2.name;
            case CulturalElement::Type::TECHNOLOGY:
                return "Integrated_" + e1.name + "_" + e2.name;
            default:
                return "Syncretic_" + e1.name + "_" + e2.name;
        }
    }

    float calculate_stability(const CulturalElement& e1,
                            const CulturalElement& e2) {
        // More stable if core importance values are similar
        float importance_diff = std::abs(e1.core_importance - e2.core_importance);
        float importance_factor = 1.0f - (importance_diff / 2.0f);
        
        // More stable if highly adaptable
        float adaptability = (e1.adaptability + e2.adaptability) / 2.0f;
        
        return (importance_factor + adaptability) / 2.0f;
    }

    float calculate_innovation_level(const CulturalElement& e1,
                                   const CulturalElement& e2) {
        // More innovative if combining different types
        float type_difference = 
            e1.type != e2.type ? 1.0f : 0.5f;
        
        // More innovative if both elements are adaptable
        float adaptability = (e1.adaptability + e2.adaptability) / 2.0f;
        
        return (type_difference + adaptability) / 2.0f;
    }

    void record_successful_combination(const SyncreticPractice& practice) {
        successful_combinations.push_back({
            practice.name,
            practice.stability,
            {"Combined " + practice.parent_elements[0] + 
             " with " + practice.parent_elements[1]}
        });
    }
}; 