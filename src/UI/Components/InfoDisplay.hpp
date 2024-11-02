#pragma once
#include "../InfoLayer/LayeredInformationSystem.hpp"
#include <godot_cpp/classes/control.hpp>

class InfoDisplay : public godot::Control {
    GDCLASS(InfoDisplay, Control)

private:
    LayeredInformationSystem::DetailLevel current_level{LayeredInformationSystem::DetailLevel::QUICK_TOOLTIP};
    LayeredInformationSystem::InfoContent current_content;
    
    // UI elements
    godot::Label* title_label{nullptr};
    godot::RichTextLabel* content_label{nullptr};
    godot::Button* more_info_button{nullptr};
    godot::Button* fun_fact_button{nullptr};

public:
    void show_info(const std::string& topic, 
                  LayeredInformationSystem::DetailLevel level = 
                      LayeredInformationSystem::DetailLevel::QUICK_TOOLTIP) {
        
        current_level = level;
        
        // Get info content
        current_content = LayeredInformationSystem::get_voting_system_info(topic);
        
        // Update display
        std::string icon = LayeredInformationSystem::get_topic_icon(topic);
        title_label->set_text((icon + " " + topic).c_str());
        
        // Show appropriate detail level
        update_content_display();
        
        // Show fun fact button if available
        std::string fun_fact = LayeredInformationSystem::get_fun_fact(topic);
        fun_fact_button->set_visible(!fun_fact.empty());
    }

private:
    void update_content_display() {
        switch (current_level) {
            case LayeredInformationSystem::DetailLevel::QUICK_TOOLTIP:
                content_label->set_text(current_content.quick_tip.c_str());
                break;
            case LayeredInformationSystem::DetailLevel::BASIC:
                content_label->set_text(current_content.basic_description.c_str());
                break;
            case LayeredInformationSystem::DetailLevel::DETAILED:
                content_label->set_text(current_content.detailed_explanation.c_str());
                break;
            case LayeredInformationSystem::DetailLevel::EDUCATIONAL:
                show_educational_content();
                break;
        }
    }

    void show_educational_content() {
        std::string text = current_content.detailed_explanation + "\n\n";
        text += "Real World Example:\n" + current_content.educational.real_world_example + "\n\n";
        text += "Historical Context:\n" + current_content.educational.historical_context + "\n\n";
        text += "Learn More:\n" + current_content.educational.further_reading;
        
        content_label->set_text(text.c_str());
    }
}; 