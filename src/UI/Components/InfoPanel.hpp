#pragma once
#include "UIComponent.hpp"
#include <godot_cpp/classes/vboxcontainer.hpp>
#include <godot_cpp/classes/label.hpp>
#include <vector>

class InfoPanel : public UIComponent {
    GDCLASS(InfoPanel, UIComponent)

private:
    godot::VBoxContainer* content_container{nullptr};
    godot::Label* title_label{nullptr};
    
    struct InfoRow {
        std::string label;
        std::string value;
        godot::Color color{colors.neutral};
    };
    std::vector<InfoRow> info_rows;

public:
    void set_title(const std::string& title) {
        title_label->set_text(title.c_str());
    }

    void add_info_row(const std::string& label, 
                     const std::string& value,
                     const godot::Color& color = godot::Color(1,1,1)) {
        info_rows.push_back({label, value, color});
        update_content();
    }

    void clear_rows() {
        info_rows.clear();
        update_content();
    }

protected:
    void _ready() override {
        setup();
    }

    void setup_theme() override {
        // Create layout
        content_container = memnew(godot::VBoxContainer);
        add_child(content_container);
        
        title_label = memnew(godot::Label);
        title_label->set_custom_minimum_size(godot::Vector2(0, 40));
        content_container->add_child(title_label);
    }

    void update_content() override {
        // Clear existing content
        for (int i = 1; i < content_container->get_child_count(); ++i) {
            content_container->get_child(i)->queue_free();
        }
        
        // Add info rows
        for (const auto& row : info_rows) {
            auto* row_container = memnew(godot::HBoxContainer);
            
            auto* label = memnew(godot::Label);
            label->set_text((row.label + ": ").c_str());
            row_container->add_child(label);
            
            auto* value = memnew(godot::Label);
            value->set_text(row.value.c_str());
            value->set_modulate(row.color);
            row_container->add_child(value);
            
            content_container->add_child(row_container);
        }
    }
}; 