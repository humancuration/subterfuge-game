#pragma once
#include <godot_cpp/classes/node.hpp>
#include <memory>
#include <unordered_map>
#include "Components/TooltipManager.hpp"
#include "Components/HighlightSystem.hpp"
#include "Components/StatusIconManager.hpp"
#include "InfoLayer/LayeredInformationSystem.hpp"

class AdvancedUISystem : public godot::Node {
    GDCLASS(AdvancedUISystem, Node)

private:
    std::unique_ptr<TooltipManager> tooltip_manager;
    std::unique_ptr<HighlightSystem> highlight_system;
    std::unique_ptr<StatusIconManager> status_icon_manager;
    std::unique_ptr<LayeredInformationSystem> info_system;

    struct UIState {
        bool is_hovering{false};
        std::string hovered_element_id;
        std::vector<std::string> active_highlights;
        float hover_time{0.0f};
    } state;

public:
    void _ready() override {
        initialize_systems();
        connect_signals();
    }

    void show_tooltip(const std::string& element_id, const godot::Vector2& position) {
        if (!state.is_hovering) {
            state.is_hovering = true;
            state.hovered_element_id = element_id;
            tooltip_manager->show_tooltip(element_id, position);
        }
    }

    void highlight_related_elements(const std::string& element_id) {
        highlight_system->highlight_connections(element_id);
        status_icon_manager->update_status_icons(element_id);
    }

    void show_detailed_info(const std::string& element_id) {
        info_system->show_info(element_id, LayeredInformationSystem::DetailLevel::DETAILED);
    }

private:
    void initialize_systems() {
        tooltip_manager = std::make_unique<TooltipManager>();
        highlight_system = std::make_unique<HighlightSystem>();
        status_icon_manager = std::make_unique<StatusIconManager>();
        info_system = std::make_unique<LayeredInformationSystem>();

        add_child(tooltip_manager.get());
        add_child(highlight_system.get());
        add_child(status_icon_manager.get());
    }
}; 