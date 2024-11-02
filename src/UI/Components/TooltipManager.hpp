#pragma once
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/label.hpp>
#include <queue>
#include "../UIConfig.hpp"

class TooltipManager : public godot::Control {
    GDCLASS(TooltipManager, Control)

private:
    struct TooltipData {
        std::string text;
        godot::Vector2 position;
        float priority;
        float duration;
    };

    godot::Label* tooltip_label{nullptr};
    std::queue<TooltipData> tooltip_queue;
    float display_timer{0.0f};
    
    const float HOVER_THRESHOLD = 0.5f; // Time before showing detailed tooltip
    const float TOOLTIP_DURATION = 3.0f;

public:
    void _ready() override {
        setup_tooltip();
    }

    void show_tooltip(const std::string& element_id, const godot::Vector2& position) {
        TooltipData data{
            get_tooltip_text(element_id),
            position,
            calculate_priority(element_id),
            TOOLTIP_DURATION
        };
        
        queue_tooltip(data);
    }

    void _process(float delta) override {
        if (!tooltip_queue.empty()) {
            display_timer += delta;
            
            if (display_timer >= tooltip_queue.front().duration) {
                tooltip_queue.pop();
                display_timer = 0.0f;
                
                if (!tooltip_queue.empty()) {
                    display_next_tooltip();
                } else {
                    hide();
                }
            }
        }
    }

private:
    void setup_tooltip() {
        tooltip_label = memnew(godot::Label);
        tooltip_label->set_autowrap(true);
        add_child(tooltip_label);
        hide(); // Start hidden
    }

    void queue_tooltip(const TooltipData& data) {
        tooltip_queue.push(data);
        update_tooltip_display();
    }

    std::string get_tooltip_text(const std::string& element_id) {
        // Get context-sensitive tooltip text based on element_id
        // This would integrate with your game's data systems
        return "Tooltip for " + element_id;
    }

    float calculate_priority(const std::string& element_id) {
        // Calculate tooltip priority based on game state and context
        return 1.0f;
    }

    void display_next_tooltip() {
        const auto& data = tooltip_queue.front();
        
        // Position tooltip near mouse but ensure it stays on screen
        godot::Vector2 screen_size = get_viewport_rect().size;
        godot::Vector2 tooltip_size = tooltip_label->get_size();
        
        godot::Vector2 final_pos = data.position;
        final_pos.x += UIConfig::Layout::TOOLTIP_OFFSET;
        final_pos.y += UIConfig::Layout::TOOLTIP_OFFSET;
        
        // Keep tooltip on screen
        if (final_pos.x + tooltip_size.x > screen_size.x) {
            final_pos.x = data.position.x - tooltip_size.x - UIConfig::Layout::TOOLTIP_OFFSET;
        }
        if (final_pos.y + tooltip_size.y > screen_size.y) {
            final_pos.y = data.position.y - tooltip_size.y - UIConfig::Layout::TOOLTIP_OFFSET;
        }
        
        set_position(final_pos);
        tooltip_label->set_text(data.text.c_str());
        
        // Fade in animation
        modulate.a = 0;
        show();
        create_tween()
            ->tween_property(this, "modulate:a", 1.0f, UIConfig::Animation::TOOLTIP_FADE_DURATION)
            ->set_ease(godot::Tween::EASE_OUT);
    }
}; 