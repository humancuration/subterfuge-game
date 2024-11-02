#pragma once
#include <godot_cpp/classes/node2d.hpp>
#include <unordered_map>
#include <vector>
#include "../UIConfig.hpp"

class HighlightSystem : public godot::Node2D {
    GDCLASS(HighlightSystem, Node2D)

private:
    struct HighlightEffect {
        godot::Color color;
        float intensity;
        float duration;
        bool pulse;
    };

    std::unordered_map<std::string, std::vector<std::string>> connection_map;
    std::unordered_map<std::string, HighlightEffect> active_highlights;

public:
    void highlight_connections(const std::string& element_id) {
        clear_highlights();
        
        // Get connected elements
        auto connected = get_connected_elements(element_id);
        
        // Create highlight effects
        for (const auto& connected_id : connected) {
            HighlightEffect effect{
                get_relationship_color(element_id, connected_id),
                1.0f,
                0.5f,
                true
            };
            active_highlights[connected_id] = effect;
        }
        
        update(); // Trigger visual update
    }

    void _draw() override {
        for (const auto& [element_id, effect] : active_highlights) {
            // Get element position from game world
            godot::Vector2 pos = get_element_position(element_id);
            float size = get_element_size(element_id);
            
            // Calculate pulse intensity
            float intensity = effect.intensity;
            if (effect.pulse) {
                float t = get_time() * UIConfig::Animation::HIGHLIGHT_PULSE_SPEED;
                intensity *= 0.7f + 0.3f * sin(t);
            }
            
            // Draw highlight circle
            godot::Color color = effect.color;
            color.a *= intensity;
            draw_circle(pos, size * 1.2f, color);
        }
    }

    void _process(float delta) override {
        bool needs_update = false;
        
        // Update highlight effects
        for (auto it = active_highlights.begin(); it != active_highlights.end();) {
            auto& effect = it->second;
            effect.duration -= delta;
            
            if (effect.duration <= 0) {
                it = active_highlights.erase(it);
                needs_update = true;
            } else {
                ++it;
            }
        }
        
        if (needs_update || has_pulsing_highlights()) {
            update();
        }
    }

private:
    std::vector<std::string> get_connected_elements(const std::string& element_id) {
        return connection_map[element_id];
    }

    godot::Color get_relationship_color(const std::string& from_id, const std::string& to_id) {
        // Determine color based on relationship type
        return godot::Color(1, 1, 1);
    }

    void clear_highlights() {
        active_highlights.clear();
        update();
    }

    bool has_pulsing_highlights() const {
        return std::any_of(active_highlights.begin(), active_highlights.end(),
            [](const auto& pair) { return pair.second.pulse; });
    }

    godot::Vector2 get_element_position(const std::string& element_id) {
        // TODO: Implement this to get actual element position from your game world
        return godot::Vector2();
    }

    float get_element_size(const std::string& element_id) {
        // TODO: Implement this to get actual element size from your game world
        return 50.0f;
    }
}; 