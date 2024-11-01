#pragma once
#include <godot_cpp/classes/node.hpp>
#include "EventSystem.hpp"
#include "../Core/LocalizationSystem.hpp"
#include <memory>
#include <string>

class ComplexEventManager : public godot::Node {
    GDCLASS(ComplexEventManager, Node)

private:
    struct EventChoice {
        alignas(16) std::string text_key;
        std::vector<std::pair<std::string, float>> effects;
        std::vector<std::string> next_events;
    };

    struct ComplexEvent {
        alignas(16) std::string id;
        alignas(16) std::string title_key;
        alignas(16) std::string description_key;
        std::vector<EventChoice> choices;
        float importance{1.0f};
    };

    std::unique_ptr<EventSystem> event_system;
    std::unique_ptr<LocalizationSystem> localization;
    
    // Cache for frequent lookups
    std::unordered_map<std::string, ComplexEvent> event_cache;
    
    // SIMD-aligned importance values for batch processing
    alignas(32) std::vector<float> event_importance;

protected:
    static void _bind_methods();

public:
    void process_events_simd();
    void handle_choice(const std::string& event_id, size_t choice_idx);
    void update_event_importance();
}; 