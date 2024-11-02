#pragma once
#include <string>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace UIConfig {
    struct Colors {
        static const godot::Color 
            PRIMARY{0.2f, 0.6f, 1.0f},
            SUCCESS{0.2f, 0.8f, 0.2f},
            WARNING{1.0f, 0.8f, 0.2f},
            ERROR{0.8f, 0.2f, 0.2f},
            NEUTRAL{0.7f, 0.7f, 0.7f},
            HIGHLIGHT{0.3f, 0.7f, 1.0f, 0.5f};
    };

    struct Animation {
        static constexpr float 
            TOOLTIP_FADE_DURATION = 0.2f,
            HIGHLIGHT_PULSE_SPEED = 2.0f,
            ICON_BOUNCE_HEIGHT = 5.0f;
    };

    struct Layout {
        static constexpr float
            TOOLTIP_OFFSET = 20.0f,
            ICON_SPACING = 24.0f,
            ICON_SIZE = 32.0f;
    };
} 