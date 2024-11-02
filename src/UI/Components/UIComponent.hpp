#pragma once
#include <godot_cpp/classes/control.hpp>
#include <string>
#include <functional>
#include "../../Utils/UIHelper.hpp"

class UIComponent : public godot::Control {
    GDCLASS(UIComponent, Control)

protected:
    UIHelper::ColorScheme colors;
    UIHelper::IconSet icons;
    
    // Common UI state
    bool is_interactive{true};
    bool is_highlighted{false};
    float animation_progress{0.0f};

public:
    virtual void setup() {
        set_anchors_preset(godot::Control::PRESET_FULL_RECT);
        setup_theme();
        connect_signals();
    }

    virtual void update(float delta) {
        update_animations(delta);
        update_content();
    }

    virtual void show_with_animation() {
        // Fade in animation
        animation_progress = 0.0f;
        set_modulate(godot::Color(1, 1, 1, 0));
        show();
        
        create_tween()
            ->tween_property(this, "modulate:a", 1.0f, 0.3f)
            ->set_ease(godot::Tween::EASE_OUT);
    }

protected:
    virtual void setup_theme() {
        // Set up default theme properties
    }

    virtual void connect_signals() {
        // Connect common signals
        connect("mouse_entered", callable_mp(this, &UIComponent::_on_mouse_entered));
        connect("mouse_exited", callable_mp(this, &UIComponent::_on_mouse_exited));
    }

    virtual void update_animations(float delta) {
        // Update any ongoing animations
    }

    virtual void update_content() {
        // Update component content
    }

    void _on_mouse_entered() {
        if (is_interactive) {
            is_highlighted = true;
            update_highlight();
        }
    }

    void _on_mouse_exited() {
        if (is_interactive) {
            is_highlighted = false;
            update_highlight();
        }
    }

    virtual void update_highlight() {
        // Update visual highlight state
    }
}; 