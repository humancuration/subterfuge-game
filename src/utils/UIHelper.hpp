#pragma once
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/vboxcontainer.hpp>
#include <string>
#include <vector>

class UIHelper {
public:
    struct ColorScheme {
        godot::Color positive{0.4f, 0.8f, 0.4f};    // Green
        godot::Color neutral{0.7f, 0.7f, 0.7f};     // Gray
        godot::Color negative{0.8f, 0.4f, 0.4f};    // Red
        godot::Color highlight{0.4f, 0.6f, 1.0f};   // Blue
        godot::Color warning{1.0f, 0.8f, 0.0f};     // Yellow
    };

    struct IconSet {
        std::string species{"🧬"};
        std::string culture{"🎭"};
        std::string technology{"⚙️"};
        std::string environment{"🌿"};
        std::string resource{"📦"};
        std::string knowledge{"📚"};
        std::string warning{"⚠️"};
    };

    static void create_progress_bar(godot::Control* parent, 
                                  const std::string& label, 
                                  float value,
                                  const godot::Color& color = ColorScheme().neutral) {
        // Create progress bar with label
    }

    static void create_info_panel(godot::Control* parent,
                                const std::string& title,
                                const std::vector<std::pair<std::string, std::string>>& info_pairs) {
        // Create information panel
    }

    static void show_tooltip(godot::Control* control,
                           const std::string& text,
                           float duration = 2.0f) {
        // Show tooltip
    }
}; 