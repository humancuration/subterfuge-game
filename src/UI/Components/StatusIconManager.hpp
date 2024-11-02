#pragma once
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <unordered_map>

class StatusIconManager : public godot::Node2D {
    GDCLASS(StatusIconManager, Node2D)

private:
    struct StatusIcon {
        godot::Ref<godot::Texture2D> texture;
        godot::Vector2 position;
        float animation_progress{0.0f};
        bool is_pulsing{false};
    };

    std::unordered_map<std::string, std::vector<StatusIcon>> element_icons;
    
public:
    void update_status_icons(const std::string& element_id) {
        clear_element_icons(element_id);
        
        // Get current status effects
        auto statuses = get_element_statuses(element_id);
        
        // Create icons for each status
        std::vector<StatusIcon> icons;
        for (const auto& status : statuses) {
            StatusIcon icon{
                get_status_texture(status),
                calculate_icon_position(element_id, icons.size()),
                0.0f,
                should_pulse(status)
            };
            icons.push_back(icon);
        }
        
        element_icons[element_id] = icons;
        update(); // Trigger visual update
    }

private:
    void clear_element_icons(const std::string& element_id) {
        element_icons.erase(element_id);
    }

    std::vector<std::string> get_element_statuses(const std::string& element_id) {
        // Get current status effects from game systems
        return {"status1", "status2"};
    }

    godot::Ref<godot::Texture2D> get_status_texture(const std::string& status) {
        // Return appropriate texture for status type
        return godot::Ref<godot::Texture2D>();
    }

    godot::Vector2 calculate_icon_position(const std::string& element_id, int icon_index) {
        // Calculate position based on element position and icon index
        return godot::Vector2();
    }

    bool should_pulse(const std::string& status) {
        // Determine if status should have pulsing animation
        return false;
    }
}; 