#pragma once
#include "UIComponent.hpp"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>

class ActionButton : public UIComponent {
    GDCLASS(ActionButton, UIComponent)

private:
    godot::Button* button{nullptr};
    godot::Label* label{nullptr};
    godot::TextureRect* icon{nullptr};
    
    std::function<void()> on_click;
    bool is_enabled{true};

public:
    void set_text(const std::string& text) {
        label->set_text(text.c_str());
    }

    void set_icon(const godot::Ref<godot::Texture2D>& texture) {
        icon->set_texture(texture);
    }

    void set_enabled(bool enabled) {
        is_enabled = enabled;
        button->set_disabled(!enabled);
        update_appearance();
    }

    void set_on_click(std::function<void()> callback) {
        on_click = callback;
    }

protected:
    void _ready() override {
        setup();
    }

    void setup_theme() override {
        // Create button layout
        button = memnew(godot::Button);
        add_child(button);
        
        auto* container = memnew(godot::HBoxContainer);
        button->add_child(container);
        
        icon = memnew(godot::TextureRect);
        container->add_child(icon);
        
        label = memnew(godot::Label);
        container->add_child(label);
        
        // Connect button signal
        button->connect("pressed", callable_mp(this, &ActionButton::_on_button_pressed));
    }

    void update_appearance() {
        float alpha = is_enabled ? 1.0f : 0.5f;
        set_modulate(godot::Color(1, 1, 1, alpha));
    }

private:
    void _on_button_pressed() {
        if (is_enabled && on_click) {
            on_click();
        }
    }
}; 