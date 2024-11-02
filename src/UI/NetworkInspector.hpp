#pragma once
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/vboxcontainer.hpp>
#include <godot_cpp/classes/progressbar.hpp>
#include <godot_cpp/classes/button.hpp>
#include "../Map/SpeciesNetworkSystem.hpp"
#include "../Map/ResourceSharingNetwork.hpp"

class NetworkInspector : public godot::Control {
    GDCLASS(NetworkInspector, Control)

private:
    // UI Elements
    godot::VBoxContainer* details_container{nullptr};
    godot::Label* title_label{nullptr};
    godot::Label* description_label{nullptr};
    godot::ProgressBar* relationship_bar{nullptr};
    godot::Button* interact_button{nullptr};

    // Data references
    SpeciesNetworkSystem* network_system{nullptr};
    ResourceSharingNetwork* resource_system{nullptr};

    struct InspectorTheme {
        struct Colors {
            godot::Color positive{0.4f, 0.8f, 0.4f};    // Green
            godot::Color neutral{0.7f, 0.7f, 0.7f};     // Gray
            godot::Color negative{0.8f, 0.4f, 0.4f};    // Red
            godot::Color highlight{0.4f, 0.6f, 1.0f};   // Blue
        } colors;

        struct Icons {
            std::string resource{"📦"};
            std::string culture{"🎭"};
            std::string environment{"🌿"};
            std::string relationship{"🤝"};
            std::string warning{"⚠️"};
        } icons;
    } theme;

public:
    void _ready() override {
        initialize_ui();
        connect_signals();
    }

    void set_network_system(SpeciesNetworkSystem* system) {
        network_system = system;
    }

    void set_resource_system(ResourceSharingNetwork* system) {
        resource_system = system;
    }

    void show_species_details(const std::string& species_id) {
        if (!network_system) return;

        clear_display();
        auto species = network_system->get_species(species_id);
        
        // Set title
        title_label->set_text(species.name);
        
        // Cultural practices section
        add_section("Cultural Practices", theme.icons.culture);
        for (const auto& practice : species.cultural_practices) {
            add_practice_item(practice);
        }
        
        // Resource usage section
        add_section("Resource Management", theme.icons.resource);
        add_resource_metrics(species);
        
        // Environmental impact section
        add_section("Environmental Impact", theme.icons.environment);
        add_environmental_metrics(species);
        
        // Relationships section
        add_section("Key Relationships", theme.icons.relationship);
        add_relationship_summary(species);
    }

    void show_relationship_details(const std::string& from_id, 
                                 const std::string& to_id) {
        if (!network_system) return;

        clear_display();
        auto relationship = network_system->get_relationship(from_id, to_id);
        
        // Set title
        title_label->set_text("Relationship Details");
        
        // Resource sharing section
        add_section("Resource Exchange", theme.icons.resource);
        add_resource_exchange_details(from_id, to_id);
        
        // Cultural exchange section
        add_section("Cultural Exchange", theme.icons.culture);
        add_cultural_exchange_details(relationship);
        
        // Environmental effects section
        add_section("Environmental Effects", theme.icons.environment);
        add_environmental_interaction_details(from_id, to_id);
        
        // Add interaction options
        add_interaction_options(relationship);
    }

private:
    void initialize_ui() {
        // Create main container
        details_container = memnew(godot::VBoxContainer);
        add_child(details_container);
        
        // Create title label
        title_label = memnew(godot::Label);
        title_label->set_custom_minimum_size(godot::Vector2(0, 40));
        details_container->add_child(title_label);
        
        // Create description label
        description_label = memnew(godot::Label);
        description_label->set_autowrap(true);
        details_container->add_child(description_label);
        
        // Create relationship bar
        relationship_bar = memnew(godot::ProgressBar);
        details_container->add_child(relationship_bar);
        
        // Create interact button
        interact_button = memnew(godot::Button);
        details_container->add_child(interact_button);
    }

    void add_section(const std::string& title, const std::string& icon) {
        auto* section_label = memnew(godot::Label);
        section_label->set_text(icon + " " + title);
        details_container->add_child(section_label);
    }

    void add_practice_item(const CulturalPractice& practice) {
        auto* practice_container = memnew(godot::HBoxContainer);
        
        auto* name_label = memnew(godot::Label);
        name_label->set_text(practice.name);
        practice_container->add_child(name_label);
        
        auto* effectiveness_bar = memnew(godot::ProgressBar);
        effectiveness_bar->set_value(practice.effectiveness * 100);
        practice_container->add_child(effectiveness_bar);
        
        details_container->add_child(practice_container);
    }

    void add_resource_metrics(const Species& species) {
        if (!resource_system) return;
        
        auto metrics = resource_system->get_resource_metrics(species.id);
        
        // Add resource production
        add_metric_bar("Production", metrics.production_rate, 
                      theme.colors.positive);
        
        // Add resource consumption
        add_metric_bar("Consumption", metrics.consumption_rate,
                      theme.colors.neutral);
        
        // Add sustainability rating
        add_metric_bar("Sustainability", metrics.sustainability_rating,
                      get_sustainability_color(metrics.sustainability_rating));
    }

    void add_environmental_metrics(const Species& species) {
        auto impact = network_system->get_environmental_impact(species.id);
        
        // Add positive contributions
        add_metric_bar("Ecosystem Support", impact.positive_contribution,
                      theme.colors.positive);
        
        // Add negative impacts
        add_metric_bar("Environmental Stress", impact.negative_impact,
                      theme.colors.negative);
        
        // Add warnings if necessary
        if (impact.negative_impact > 0.7f) {
            add_warning("High environmental impact detected!");
        }
    }

    void add_metric_bar(const std::string& label, float value, 
                       const godot::Color& color) {
        auto* bar = memnew(godot::ProgressBar);
        bar->set_value(value * 100);
        bar->set_modulate(color);
        details_container->add_child(bar);
    }

    void add_warning(const std::string& message) {
        auto* warning_label = memnew(godot::Label);
        warning_label->set_text(theme.icons.warning + " " + message);
        warning_label->set_modulate(theme.colors.negative);
        details_container->add_child(warning_label);
    }

    godot::Color get_sustainability_color(float rating) {
        if (rating > 0.7f) return theme.colors.positive;
        if (rating > 0.4f) return theme.colors.neutral;
        return theme.colors.negative;
    }

    void clear_display() {
        for (int i = details_container->get_child_count() - 1; i >= 0; --i) {
            details_container->get_child(i)->queue_free();
        }
    }

protected:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("show_species_details", "species_id"),
                           &NetworkInspector::show_species_details);
        ClassDB::bind_method(D_METHOD("show_relationship_details", "from_id", "to_id"),
                           &NetworkInspector::show_relationship_details);
    }
}; 